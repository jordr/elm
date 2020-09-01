/*
 *	Plugger class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-07, IRIT UPS.
 *
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "../config.h"
#include <elm/deprecated.h>
#include <elm/assert.h>
#include <stdlib.h>
#if defined(__WIN32) || defined(__WIN64)
#	include <windows.h>
#	undef min
#	undef max
	namespace elm { namespace win {
		void setError(int code);
		string getErrorMessage(void);
	} }	// elm::win
#elif defined(WITH_LIBTOOL)
#	include <ltdl.h>
#else
#	include <dlfcn.h>
#endif
#include <elm/sys/Plugger.h>
#include <elm/sys/System.h>
#include <elm/io.h>
#include <elm/ini.h>
#include <elm/util/UniquePtr.h>

namespace elm { namespace sys {

static cstring ELD_EXT = "eld";
static cstring
	SECTION_NAME = "elm-plugin",
	// NAME_ATT = "name",
	PATH_ATT = "path",
	// DESCRIPTION = "description",
	RPATH_ATT = "rpath",
	LIBS_ATT = "libs",
	DEPS_ATT = "deps";

static cstring PLUG_EXT =
#	ifdef WITH_LIBTOOL
		"la";
#	elif defined(__WIN32) || defined(__WIN64)
		"dll";
#	elif defined(__APPLE__)
		"dylib";
#	else
		"so";
#	endif
static cstring fun_suffix = "_fun";


/**
 * Test if the given file is a library.
 * @param file	File to test.
 * @return		True if it is a library, false else.
 */
static inline bool isLibrary(LockPtr<FileItem> file) {
#if defined(WITH_LIBTOOL)
		return file->path().toString().endsWith(".la");
#elif defined(__APPLE__)
		return file->path().toString().endsWith(".dylib");
#elif defined(__unix)
		return file->path().toString().endsWith(".so");
#elif defined(__WIN32) || defined(__WIN64)
		return file->path().toString().endsWith(".dll");
#endif
}


/**
 * @class Plugger
 * This class is used for connecting Plugin object from dynamic loaded code
 * unit. The Plugin object is retrieved as a global data matching the hook name
 * passed to the plugger creation. The dynamic loaded code units are retrieved
 * from the paths given to the plugger object. See @ref Plugin.
 * @ingroup plugins
 */


/**
 * Current active pluggers.
 */
Vector<Plugger *> Plugger::pluggers;


/**
 * Build a new plugger.
 * @param hook				Hook name for retrieving plugin in the code unit.
 * @param plugger_version	Plugger version for compatibility checking.
 * @param _paths			List of path separated by ":" for retrieving
 * 							the plugin. Pass "*" for initializing with the
 * 							default system paths.
 */
Plugger::Plugger(CString hook, const Version& plugger_version, String paths)
: _hook(hook), per_vers(plugger_version), err(OK), _quiet(false) {

	// Initialize DL library
	#if !defined(__WIN32) && !defined(__WIN64) && defined(WITH_LIBTOOL)
		static bool preloaded = false;
		if(!preloaded) {
			lt_dlinit();
			preloaded = true;
		}
	#endif

	// Look in the system paths
	if(paths == "*")
#if defined(__APPLE__)
		paths = getenv("DYLD_LIBRARY_PATH");
#else
		paths = getenv("LD_LIBRARY_PATH");
#endif

	// Scan the paths
	int index = paths.indexOf(elm::sys::Path::PATH_SEPARATOR);
	while(index >= 0) {
		if(index)
			_paths.add(paths.substring(0, index));
		paths = paths.substring(index + 1);
		index = paths.indexOf(elm::sys::Path::PATH_SEPARATOR);
	}
	if(paths)
		_paths.add(paths);

	// Add to active pluggers
	pluggers.add(this);
}


/**
 */
Plugger::~Plugger(void) {
	pluggers.remove(this);
	#if !defined(__WIN32) && !defined(__WIN64) && defined(WITH_LIBTOOL)
		lt_dlexit();
	#endif
}


/**
 * For internal use only.
 */
void Plugger::leave(Plugin *plugin) {
	for(int i = 0; i < pluggers.count(); i++)
		pluggers[i]->plugins.remove(plugin);
}


/**
 * Add new path for retrieving plugins.
 * @param path	Added path.
 */
void Plugger::addPath(String path) {
	_paths.add(path);
}


/**
 * Remove a path.
 * @param path	Removed path.
 */
void Plugger::removePath(String path) {
	_paths.remove(path);
}


/**
 * Remove all paths.
 */
void Plugger::resetPaths(void) {
	_paths.clear();
}


/**
 * Get a plugin by its name.
 * @param name	Name of the plugin to get (possibly containing '/'
 * 				to access sub-directories).
 * @return		Found plugin or null.
 */
Plugin *Plugger::plug(const string& name) {
	err = OK;

	// Is it a path ?
	if(name.startsWith("/"))
		return plugFile(name);

	// Look in opened plugins
	for(int i = 0; i < plugins.count(); i++)
		if(plugins[i]->matches(name)) {
			plugins[i]->plug(nullptr);
			return plugins[i];
		}

	// Look in static plugins
	Plugin *plugin = Plugin::get(_hook, name);
	if(plugin)
		return plug(plugin, nullptr);

	// Load the plugin
	for(int i = 0; i < _paths.count(); i++) {
		StringBuffer buf;
		buf << _paths[i] << "/" << name << "." << PLUG_EXT;
		error_t old_err = err;
		Plugin *plugin = plugFile(buf.toString());
		if(plugin)
			return plugin;
		if(old_err != OK && err == NO_PLUGIN)
			err = old_err;
	}

	// No plugin available
	return nullptr;
}



/**
 * Test if the named plug-in is plugged.
 * @param name	Name of the plug-in.
 * @return		True if the plug-in is plugged, false else.
 */
bool Plugger::isPlugged(string name) const {
	for(auto p: plugged()) {
		if(p->name() == name)
			return true;
		for(auto n: p->aliases())
			if(n == name)
				return p;
	}
	return false;
}


/**
 */
Plugin *Plugger::plug(Plugin *plugin, void *handle) {
	plugin->plug(handle);
	if(!plugins.contains(plugin))
		plugins.add(plugin);
	return plugin;
}


/**
 * Evaluate the given path. For now, replaced only prefix "$ORIGIN" by current plugin
 * path.
 * @param plugin_path	Path to the current plugin.
 * @param path			Path to transform.
 * @return				Transformed path.
 */
static inline sys::Path evaluate(sys::Path plugin_path, sys::Path path) {
	string p = path;
	if(p.startsWith("$ORIGIN"))
		return plugin_path.dirPart() / p.substring(8);
	else
		return path;
}


/**
 * Link an OS library.
 * @param lib		Library to link (OS extension is automatically added if required).
 * @return			Opened library or null.
 */
void *Plugger::link(sys::Path lib) {
#	if defined(__WIN32) || defined(__WIN64)
		return LoadLibrary(lib.asSysString());
#	elif defined(WITH_LIBTOOL)
		return lt_dlopen(lib.asSysString());
#	else
		return dlopen(lib.asSysString(), RTLD_LAZY);
#	endif
}


/**
 * Look and load a library.
 * @param lib	Library path (OS extension automatically added).
 * @param rpath	List of paths to look in (if empty, look in OS paths).
 * @return		Hook on the library.
 */
void *Plugger::lookLibrary(sys::Path lib, Vector<string> rpath) {
	lib = lib.setExtension(PLUG_EXT);
	if(lib.isAbsolute() || !rpath)
		return link(lib);
	else {
		for(int i = 0; i < rpath.count(); i++) {
			void *handle = link(sys::Path(rpath[i]) / lib);
			if(handle)
				return handle;
		}
		return 0;
	}
}


/**
 * Look for a symbol in the given library.
 * @param handle	Handle of the library to look in.
 * @param name		Name of the looked symbol.
 * @return			Address of the symbol or null.
 */
void *Plugger::lookSymbol(void *handle, cstring name) {
#	if defined(__WIN32) || defined(__WIN64)
		return (void *)(GetProcAddress(reinterpret_cast<HINSTANCE&>(handle), name.chars()));
#	elif defined(WITH_LIBTOOL)
		return lt_dlsym((lt_dlhandle)handle, name.chars());
#	else
		return dlsym(handle, name.chars());
#	endif
}


/**
 * Look for an ELD file and process it.
 * @param path	Path to plugin.
 * @param err	To return error.
 * @return		Opened plugin if any.
 */
Plugin *Plugger::lookELD(const Path& path, error_t& err, Vector<Plugin *>& _deps) {
	_deps.clear();

	// compute ELD path
	sys::Path ppath = path;
	if(ppath.extension() == PLUG_EXT)
		ppath = ppath.setExtension(ELD_EXT);
	else if(ppath.extension() != ELD_EXT)
		ppath = _ << ppath << "." << ELD_EXT;

	try {

		// open the ELD file
		UniquePtr<ini::File> file(ini::File::load(ppath));
		ini::Section *sect = file->get(SECTION_NAME);

		// if section is provided
		if(sect) {

			// just renaming
			sys::Path npath = sect->get(PATH_ATT);
			if(npath)
				return plugFile(evaluate(ppath, npath).setExtension(PLUG_EXT));

			// pre-link other plugins
			Vector<string> deps;
			sect->getList(DEPS_ATT, deps);
			for(int i = 0; i < deps.count(); i++) {
				Plugin *plugin = plug(evaluate(ppath, deps[i]));
				if(plugin)
					_deps.add(plugin);
				else {
					onError(level_error, _ << "cannot plug " << deps[i]);
					err = MISSING_DEP;
					return 0;
				}
			}

			// pre-link libraries
			Vector<string> libs;
			sect->getList(LIBS_ATT, libs);
			if(libs) {

				// add the RPATH if any
				Vector<string> rpaths;
				sect->getList(RPATH_ATT, rpaths);
				for(int i = 0; i < rpaths.count(); i++)
					rpaths[i] = evaluate(ppath, rpaths[i]);
				if(!rpaths)
					rpaths.add(ppath.parent());

				// link the libraries
				for(int i = 0; i < libs.count(); i++)
					if(!lookLibrary(evaluate(ppath, libs[i]), rpaths)) {
						onError(level_error, _ << "cannot link " << libs[i]);
						err = MISSING_DEP;
						return 0;
					}
			}
		}
	}
	catch(ini::Exception& e) {
	}
	return 0;
}


/**
 * Plug the given file in the plugger.
 * @param path	Path of file to plug.
 * @return		Plugin or null if there is an error.
 */
Plugin *Plugger::plugFile(sys::Path path) {
	Plugin::static_done = true;
	err = OK;

	// add extension if not present
	if(path.extension() != PLUG_EXT)
		path = path.setExtension(PLUG_EXT);

	// look for ELD file
	Vector<Plugin *> deps;
	Plugin *res = lookELD(path, err, deps);
	if(err != OK)
		return 0;
	if(res)
		return res;

	// Check existence of the file
	if(!path.exists()) {
		return nullptr;
	}
	else if(!path.isFile()) {
		onWarning(_ << path << " is not a file!");
		err = NO_PLUGIN;
		return nullptr;
	}
	else if(!path.isReadable()) {
		onWarning(_ << path << " is not accessible!");
		err = NO_PLUGIN;
		return nullptr;
	}
	
	/* TOFIX -- disabled for now as it seems to be too strict
	 * Should be re-enabled in future.
	else if(!path.isExecutable()) {
		onWarning(_ << path << " is not executable!");
		err = NO_PLUGIN;
		return nullptr;
	}*/

#	if defined(__WIN32) || defined(__WIN64)
		Path rpath = path;
		rpath = rpath.setExtension("link");
		if(rpath.exists() && rpath.isFile() && rpath.isReadable()) {
			io::InStream *in = 0;
			try {
				in = System::readFile(rpath);
				io::Input input(*in);
				String npath;
				input >> npath;
				path = rpath.parent() / npath;
			}
			catch(io::IOException& e) {
				onError(level_error, e.message());
				if(in)
					delete in;
			}
		}
#	endif

	// Open shared library
	void *handle = link(path);
	if(!handle) {
		err = BAD_PLUGIN;
		onError(level_warning, _ << "invalid plugin found at \"" << path << "\" (no handle): " << error());
		return 0;
	}

	// new version: look for builder function
	Plugin *plugin = 0;
	string fun_name = _ << _hook << fun_suffix;
	void *sym = lookSymbol(handle, fun_name.toCString());
	if(sym) {
		typedef Plugin *(*fun_t)(void);
		fun_t fun = (fun_t)sym;
		plugin = fun();
	}

	// old version: look for the static object
	else {
		void *sym = lookSymbol(handle, _hook.chars());
		if(sym)
			plugin = static_cast<Plugin *>(sym);
	}

	// plugin found?
	if(!plugin) {
		err = NO_HOOK;
		unlink(handle);
		onError(level_warning, _ << "invalid plugin found at \"" << path << "\" (no hook)");
		return 0;
	}

	// Check the magic
	if(plugin->magic != Plugin::MAGIC) {
		err = NO_MAGIC;
		unlink(handle);
		onError(level_warning, _ << "invalid plugin found at \"" << path << "\" (bad magic)");
		return 0;
	}

	// Check plugger version
	if(!per_vers.accepts(plugin->pluggerVersion())) {
		err = BAD_VERSION;
		unlink(handle);
		onError(level_warning, _ << "bad version plugin found at \"" << path << "\" (required: " << per_vers << ", provided: " << plugin->pluggerVersion() << ")");
		return 0;
	}

	// Plug it
	plugin->setPath(path);
	for(Vector<Plugin *>::Iter dep(deps); dep(); dep++)
		plugin->deps.add(*dep);
	return plug(plugin, handle);
}


/**
 * @fn error_t Plugger::lastError(void);
 * Get the last error.
 * @return	Last error.
 */
Plugger::error_t Plugger::lastError(void) {
	DEPRECATED
	return err;
}


/**
 * Get the message for the last error.
 * @return	Error message.
 * @deprecated
 */
String Plugger::lastErrorMessage(void) {
	DEPRECATED
	return getLastError();
}


/**
 * Return the last produced error.
 * @return		Last error message.
 * @deprecated
 */
string Plugger::getLastError() {
	switch(err) {
	case OK:
		return "Success.";
	case BAD_PLUGIN: {
			StringBuffer buf;
			#if defined(__WIN32) || defined(__WIN64)
				buf << "cannot open the plugin(" << GetLastError() << ").";
			#elif defined(WITH_LIBTOOL)
				const char *msg = lt_dlerror();
				buf << "cannot open the plugin(" << (msg ? msg : "") << ").";
			#else
				buf << "cannot open the plugin(" << dlerror() << ").";
			#endif
			return buf.toString();
		}
	case NO_PLUGIN:
		return "cannot find any plugin matching the given name";
	case NO_HOOK:
		return "Found plugin does not contain a hook symbol.";
	case BAD_VERSION:
		return "Found plug-in is incompatible.";
	case NO_MAGIC:
		return "Found plugin does not match the plugin signature.";
	default:
		ASSERTP(0, "unknown error");
		return "";
	}
}


/**
 * @fn bool Plugger::quiet(void) const;
 * Test if the plugger emit or not message error during plugin operation.
 * @return	True for quiet, false else.
 */


/**
 * @fn void Plugger::setQuiet(bool quiet);
 * Set the quiet mode of the plugger. If set to true, the plugger
 * will no more emit error messages during plugin operation.
 * @param quiet	True for quiet mode, false else.
 */


/**
 * This method is called when an error arises to let the user display or not
 * the message. As default, the message is displayed on standard error.
 * @param message	Message of the error.
 * @deprecated
 */
void Plugger::onError(String message) {
	if(!_quiet)
		cerr << "ERROR: " << message << io::endl;
}


/**
 * Generate an error for the error handler.
 * @param level		Level of error.
 * @param message	Error message.
 */
void Plugger::onError(error_level_t level, const string& message) {
	if(_quiet)
		return;
	ErrorBase::onError(level, message);
	if(level == level_error)
		onError(message);
	else if(level == level_warning)
		onWarning(message);
}


/**
 * This method is called when a warning arises to let the user display or not
 * the message. As default, the message is displayed on standard error.
 * @param message	Message of the warning.
 * @deprecated
 */
void Plugger::onWarning(String message) {
	if(!_quiet)
		cerr << "WARNING: " << message << io::endl;
}


/**
 * @class Plugger::Iterator
 * Used for exploring plugins hookable on the current plugger.
 */


/**
 */
void Plugger::Iter::go(void) {

	// Look in statics
	if(i < statics.count()) {
		i++;
		while(i < c)  {
			if(statics[i]->hook() == plugger.hook())
				return;
			i++;
		}
	}

	// Look in files
	while(true) {

		// Next file
		if(file)
			file->next();

		// Next path
		if(!file || file->ended()) {
			if(file) {
				delete file;
				file = 0;
			}
			_path++;
			if(_path >= plugger._paths.count())
				break;
			LockPtr<FileItem> item = FileItem::get(Path(plugger._paths[_path]));
			if(!item || !item->toDirectory())
				continue;
			else {
				file = new Directory::Iter(item->toDirectory());
				if(file->ended())
					continue;
			}
		}

		// Look current file
		if(isLibrary(**file)) {
			Plugin *plugin = plugger.plugFile(file->item()->path());
			if(plugin) {
				plugin->unplug();
				break;
			}
		}
	}

}


/**
 * Build a new iterator.
 * @param plugger	Used plugger.
 * @param bool		If true, builds an end iterator (default to false).
 */
Plugger::Iter::Iter(Plugger& _plugger, bool end)
:	plugger(_plugger),
	statics(_plugger.statics()),
	i(-1),
	c(_plugger.statics().count()),
	_path(-1),
	file(0)
{
	if(!end)
		go();
	else
		_path = plugger._paths.count();
}


/**
 */
Plugger::Iter::~Iter(void) {
	if(file)
		delete file;
}


/**
 * Test if the iteration is ended.
 * @return	True if it is ended.
 */
bool Plugger::Iter::ended(void) const {
	return _path >= plugger._paths.count();
}


/**
 * Get the current plugin name.
 * @return	Current plugin name.
 */
String Plugger::Iter::item(void) const {
	if(i < c)
		return statics[i]->name();
	else {
		Path path = (*file)->path();
		String name = path.namePart();
		name = name.substring(0, name.length() - 3);
		return name;
	}
}


/**
 * Get the path of the current plug-in.
 * @return	Plug-in path.
 */
Path Plugger::Iter::path(void) const {
	if(i < c)
		return "<static>";
	else
		return (*file)->path();
}


/**
 * Go to the next plugin.
 */
void Plugger::Iter::next(void) {
	go();
}


/**
 * Plug the current plugin.
 * @return	Matching plugin.
 */
Plugin *Plugger::Iter::plug(void) const {
	if(i < statics.count())
		return plugger.plug(statics[i], 0);
	else
		return plugger.plugFile(file->item()->path().toString());
}


/**
 * Get error message from the last library operation.
 * @return	Last error message.
 */
string Plugger::error(void) {
#	if defined(__WIN32) || defined(__WIN64)
		win::setError(GetLastError());
		return win::getErrorMessage();
#	elif defined(WITH_LIBTOOL)
		return lt_dlerror();
#	else
		return dlerror();
#	endif
}


/**
 * Unlink a library (OS level).
 * @param handle	Library handle.
 */
void Plugger::unlink(void *handle) {
#	ifdef WITH_LIBTOOL
		lt_dlclose((lt_dlhandle)_handle);
#	elif defined(__WIN32) || defined(__WIN64)
		if(FreeLibrary(reinterpret_cast<HINSTANCE&>(handle)) == 0 )
			throw SystemException(errno,"error freeing library");
#	else
		dlclose(handle);
#	endif
}


/**
 * @fn const Vector<string>& Plugger::paths() const;
 * Get the list of direcory paths used to retrieve plug-ins.
 * @return	List of plug-ins paths.
 */


/**
 * @fn Range<Iter> Plugger::available();
 * Get the list of the names of available plug-ins (may be not already plugged).
 * @return	List of names of the available plug-ins.
 */


/**
 * @fn const Vector<Plugin *>& plugged() const;
 * Get the list of plugged plug-ins.
 * @return	Plugged plug-ins.
 */

} }	// elm::sys
