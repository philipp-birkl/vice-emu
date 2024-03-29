ACLOCAL_AMFLAGS=-Im4

if VICE_QUIET
.SILENT:
endif

# Process this file with automake to produce Makefile.in

SUBDIRS = src build data man doc

# COMMON_EXTRA_DIST is what used to be the EXTRA_DIST, when adding dist
# files, please add them to the COMMON_EXTRA_DIST list.

COMMON_EXTRA_DIST = \
	AUTHORS \
	autogen.sh \
	config.rpath \
	configure.proto \
	ChangeLog \
	ChangeLog.1 \
	ChangeLog.2 \
	ChangeLog.3 \
	ChangeLog.4 \
	COPYING \
	FEEDBACK \
	INSTALL \
	NEWS

EXTRA_DIST = $(COMMON_EXTRA_DIST)

if SUPPORT_X64
  X64_INCLUDED = yes
else
  X64_INCLUDED = no
endif


.PHONY: vsid x64 x64sc x128 x64dtv xvic xpet xplus4 xcbm2 xcbm5x0 xscpu64 c1541 petcat cartconv
vsid:
	(cd src; $(MAKE) vsid-all)

if SUPPORT_X64
x64:
	(cd src; $(MAKE) x64-all)
endif

x64sc:
	(cd src; $(MAKE) x64sc-all)

x128:
	(cd src; $(MAKE) x128-all)

x64dtv:
	(cd src; $(MAKE) x64dtv-all)

xscpu64:
	(cd src; $(MAKE) xscpu64-all)

xvic:
	(cd src; $(MAKE) xvic-all)

xpet:
	(cd src; $(MAKE) xpet-all)

xplus4:
	(cd src; $(MAKE) xplus4-all)

xcbm2:
	(cd src; $(MAKE) xcbm2-all)

xcbm5x0:
	(cd src; $(MAKE) xcbm5x0-all)

c1541:
	(cd src; $(MAKE) c1541-all)

petcat:
	(cd src; $(MAKE) petcat-all)

cartconv:
	(cd src; $(MAKE) cartconv)

ffmpeglibs:
	(cd src/lib; $(MAKE) ffmpeglibs)

ffmpeglibs-install:
	(cd src/lib; $(MAKE) ffmpeglibs-install)

ffmpeglibs-clean:
	(cd src/lib; $(MAKE) clean)

install: installvice

# If we have no ChangeLog (which should be generated from SVN), generate a
# dummy one.

ChangeLog:
	touch ChangeLog

if MAKE_BINDIST

if AMIGA_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_amiga.sh $(STRIP) $(VICE_VERSION) $(host_cpu) $(host_os) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir) $(SDL_VERSION) $(EXEEXT)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_amiga.sh $(STRIP) $(VICE_VERSION) $(host_cpu) $(host_os) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir) $(SDL_VERSION) $(EXEEXT)
endif
endif

if BEOS_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_beos.sh $(STRIP) $(VICE_VERSION) $(host_cpu) $(host_os) @ENABLE_ARCH@ nozip $(XSCPU64_INCLUDED) $(top_srcdir) $(SDL_VERSION)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_beos.sh $(STRIP) $(VICE_VERSION) $(host_cpu) $(host_os) @ENABLE_ARCH@ zip $(XSCPU64_INCLUDED) $(top_srcdir) $(SDL_VERSION)
endif
endif

if UNIX_MACOSX_COMPILE
if NATIVE_GTK3_COMPILE
bindist:
	/bin/bash @ARCH_SRC_DIR@/../gtk3/make-bindist_osx.sh $(top_srcdir) $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip

bindistzip:
	/bin/bash @ARCH_SRC_DIR@/../gtk3/make-bindist_osx.sh $(top_srcdir) $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip
else
bindist:
	FW_DIR=@FW_DIR@ /bin/bash @ARCH_SRC_DIR@/../sdl/make-bindist_osx.sh $(top_srcdir) $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(SDL_VERSION)

bindistzip:
	FW_DIR=@FW_DIR@ /bin/bash @ARCH_SRC_DIR@/../sdl/make-bindist_osx.sh $(top_srcdir) $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(SDL_VERSION)
endif
endif


if WIN32_COMPILE
if NATIVE_GTK3_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_win32.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(X64_INCLUDED) $(top_srcdir) $(host_cpu) $(abs_top_builddir) $(CROSS) $(objdump) $(CC)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_win32.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(X64_INCLUDED) $(top_srcdir) $(host_cpu) $(abs_top_builddir) $(CROSS) $(objdump) $(CC)
else
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_win32.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(X64_INCLUDED) $(top_srcdir) $(top_builddir) $(host_cpu) $(SDL_VERSION)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_win32.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(X64_INCLUDED) $(top_srcdir) $(top_builddir) $(host_cpu) $(SDL_VERSION)
endif
endif
endif

else
bindist:
	@echo this platform doesn\'t use \"make bindist\", use \"make install\" instead
endif

if HAVE_SHARED_FFMPEG
EXTRA_INSTALLS = ffmpeglibs-install
EXTRA_CLEANS = ffmpeglibs-clean
else
if HAVE_STATIC_FFMPEG
EXTRA_INSTALLS =
EXTRA_CLEANS = ffmpeglibs-clean
else
EXTRA_INSTALLS =
EXTRA_CLEANS =
endif
endif

if MAKE_INSTALL
installvice: install-recursive $(EXTRA_INSTALLS)
else
installvice:
	@echo this platform doesn\'t use \"make install\", use \"make bindist\" instead
endif

# Manually define the dist targets so the correct tar usage will be used.
dist-gzip:
	$(DOS2UNIX) `find $(top_srcdir) -name "*.sh"`
	$(DOS2UNIX) $(top_srcdir)/doc/vice.texi
	$(MAKE) distdir
	$(AMTAR) chf - $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	$(am__remove_distdir)

dist dist-all:
	$(DOS2UNIX) `find $(top_srcdir) -name "*.sh"`
	$(DOS2UNIX) $(top_srcdir)/doc/vice.texi
	$(MAKE) distdir
	$(AMTAR) chf - $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	$(am__remove_distdir)

$(top_srcdir)/configure.ac: $(top_srcdir)/configure.proto
	$(am__cd) $(srcdir) && $(SHELL) autogen.sh

clean: $(EXTRA_CLEANS) clean-recursive
