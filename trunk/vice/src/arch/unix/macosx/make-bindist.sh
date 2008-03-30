#!/bin/sh
# make-bindist.sh for Mac OSX
# written by Christian Vogelgsang <chris@vogelgsang.org>
#
# make-bindist.sh <top_srcdir> <strip> <vice-version> <zip|nozip>

RUN_PATH=`dirname $0`

echo "Generating Mac OSX binary distribution."

TOP_DIR=$1
STRIP=$2
VICE_VERSION=$3
ZIP=$4

# check binary type
TEST_BIN=src/x64
if [ ! -x $TEST_BIN ]; then
	echo "error missing binary $TEST_BIN"
	exit 1
fi
BIN_TYPE=`file $TEST_BIN | grep "$TEST_BIN:" | cut -f3,4 -d" "`
if [ x"$BIN_TYPE" = "xuniversal binary" ]; then
	BIN_FORMAT=ub
elif [ x"$BIN_TYPE" = "xfat file" ]; then
  BIN_FORMAT=ub
elif [ x"$BIN_TYPE" = "xexecutable i386" ]; then
	BIN_FORMAT=i386
elif [ x"$BIN_TYPE" = "xexecutable ppc" ]; then
	BIN_FORMAT=ppc
else
	echo "fatal: unknown bin type '$BIN_TYPE'"
	exit 1
fi
echo "  binary format: $BIN_FORMAT"

# setup BUILD dir
BUILD_DIR=vice-macosx-x11-$BIN_FORMAT-$VICE_VERSION
if [ -d $BUILD_DIR ]; then
	rm -rf $BUILD_DIR
fi
mkdir $BUILD_DIR
if [ ! -d $BUILD_DIR ]; then
	echo "error creating directory $BUILD_DIR"
	exit 1
fi

# make tools dir
TOOL_DIR=$BUILD_DIR/tools
if [ ! -d $TOOL_DIR ]; then
	mkdir $TOOL_DIR
fi

# define bundles and command line tools
BUNDLES="x64 x128 xcbm2 xpet xplus4 xvic"
TOOLS="c1541 petcat cartconv"

# define data files for emulators
ROM_COMMON="DRIVES PRINTER"
ROM_x64=C64
ROM_x128=C128
ROM_xcbm2=CBM-II
ROM_xpet=PET
ROM_xplus4=PLUS4
ROM_xvic=VIC20
# files to remove from ROM directory
ROM_REMOVE="Makefile* {beos,amiga,dos,os2,win}*.vkm"
DOC_REMOVE="Makefile* texi2html *.tex *.texi MSDOS* Minix* *.beos *.dos Win32*"

# --- create each bundle ---
for bundle in $BUNDLES ; do
  echo -n "  bundling app $bundle.app: "	
  
  APP_NAME=$BUILD_DIR/${bundle}.app
  APP_CONTENTS=$APP_NAME/Contents
  APP_MACOS=$APP_CONTENTS/MacOS
  APP_RESOURCES=$APP_CONTENTS/Resources
  APP_ROMS=$APP_RESOURCES/ROM
  APP_DOCS=$APP_RESOURCES/doc
  
  # create directory structure
	echo -n "[app dirs] "
	mkdir -p $APP_CONTENTS
	mkdir -p $APP_MACOS
	mkdir -p $APP_RESOURCES
	mkdir -p $APP_ROMS
	mkdir -p $APP_DOCS

	# copy icons
	echo -n "[icons] "
	if [ ! -e $RUN_PATH/VICE.icns ]; then
		echo "ERROR: missing icon: $RUNPATH/VICE.icns"
		exit 1
	fi
	cp $RUN_PATH/VICE.icns $APP_RESOURCES/

	# setup Info.plist
	echo -n "[Info.plist] "
	if [ ! -e $RUN_PATH/Info.plist ]; then
		echo "ERROR: missing: $RUN_PATH/Info.plist"
		exit 1
	fi
	sed -e "s/XNAMEX/$bundle/g" -e "s/XVERSIONX/$VICE_VERSION/g" \
  	< $RUN_PATH/Info.plist > $APP_CONTENTS/Info.plist

	# create launcher
	echo -n "[launcher] "
	LAUNCHER=$APP_MACOS/$bundle
	echo '#!/bin/bash' > $LAUNCHER
	echo 'RUNPATH=`dirname $0`' >> $LAUNCHER
	echo "/usr/bin/open-x11 \$RUNPATH/${bundle}.xterm" >> $LAUNCHER
	chmod 755 $LAUNCHER
	TERMINAL=$APP_MACOS/$bundle.xterm
	echo '#!/bin/bash' > $TERMINAL
	echo 'RUNPATH=`dirname $0`' >> $TERMINAL
	echo 'export PATH=$RUNPATH:/usr/X11R6/bin:$PATH' >> $TERMINAL
	echo "exec /usr/X11R6/bin/xterm -sb -title \"VICE ${bundle} Console\" -e \$RUNPATH/${bundle}.bin" >> $TERMINAL
	chmod 755 $TERMINAL

	# copy binary
	echo -n "[binary] "
	if [ ! -e src/$bundle ]; then
		echo "ERROR: missing binary: src/$bundle"
		exit 1
	fi
	cp src/$bundle $APP_MACOS/$bundle.bin
	
	# strip binary
	if [ x"$STRIP" = "xstrip" ]; then
		echo -n "[strip] "
		/usr/bin/strip $APP_MACOS/$bundle.bin
	fi

	# copy roms and data into bundle
	echo -n "[common ROMs] "
	for rom in $ROM_COMMON ; do
		if [ ! -d $TOP_DIR/data/$rom ]; then
			echo "ERROR: missing ROM: $TOP_DIR/data/$rom"
			exit 1
		fi
		cp -r $TOP_DIR/data/$rom $APP_ROMS/
		(cd $APP_ROMS/$rom && eval "rm -f $ROM_REMOVE")
  done
  eval "ROM=\${ROM_$bundle}"
  echo -n "[ROM=$ROM] "
	if [ ! -d $TOP_DIR/data/$ROM ]; then
		echo "ERROR: missing ROM: $TOP_DIR/data/$ROM"
		exit 1
	fi
  cp -r $TOP_DIR/data/$ROM $APP_ROMS/
	(cd $APP_ROMS/$ROM && eval "rm -f $ROM_REMOVE")
  
  # copy html docs into bundle
  echo -n "[docs] "
  cp -r $TOP_DIR/doc/html/* $APP_DOCS/
	(cd $APP_DOCS && eval "rm -f $DOC_REMOVE")
  
	# embed c1541
	echo -n "[c1541] "
	if [ ! -e src/c1541 ]; then
		echo "ERROR: missing binary: src/c1541"
		exit 1
	fi
	cp src/c1541 $APP_MACOS/

	# strip embedded c1541 binary
	if [ x"$STRIP" = "xstrip" ]; then
		echo -n "[strip c1541] "
		/usr/bin/strip $APP_MACOS/c1541
	fi

  # ready
  echo
done

# --- copy tools ---
for tool in $TOOLS ; do
	echo -n "  copying tool $tool: "

	# copy binary
	echo -n "[binary] "
	if [ ! -e src/$tool ]; then
		echo "ERROR: missing binary: src/$bundle"
		exit 1
	fi
	cp src/$tool $TOOL_DIR/
	
	# strip binary
	if [ x"$STRIP" = "xstrip" ]; then
		echo -n "[strip] "
		/usr/bin/strip $TOOL_DIR/$tool
	fi

	# ready
	echo
done

# --- copy docs ---
echo "  copying documents"
cp $TOP_DIR/FEEDBACK $BUILD_DIR/FEEDBACK.txt
cp $TOP_DIR/README $BUILD_DIR/README.txt
cp -r $TOP_DIR/doc $BUILD_DIR/
find $BUILD_DIR/doc -name "Makefile*" -exec rm {} \;
(cd $BUILD_DIR/doc && eval "rm -f $DOC_REMOVE")

# --- make dmg? ---
if [ x"$ZIP" = "xnozip" ]; then
	echo "ready. created dist directory: $BUILD_DIR"
	du -sh $BUILD_DIR
else
	# image name
	BUILD_IMG=$BUILD_DIR.dmg
	BUILD_TMP_IMG=$BUILD_DIR.tmp.dmg
	
	# Create the image and format it
	echo "  creating DMG"
	hdiutil create -srcfolder $BUILD_DIR $BUILD_TMP_IMG -volname $BUILD_DIR -ov -quiet 

	# Compress the image
	echo "  compressing DMG"
	hdiutil convert $BUILD_TMP_IMG -format UDZO -o $BUILD_IMG -ov -quiet
	rm -f $BUILD_TMP_IMG

	# Zip image to compress it even more
	echo "  gzipping DMG"
	gzip -9 -f $BUILD_IMG

	echo "ready. created dist file: $BUILD_IMG"
	du -sh $BUILD_IMG.gz
	md5 -q $BUILD_IMG.gz
fi
