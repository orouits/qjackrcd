# OVERVIEW

QJackRcd is a simple QT application to record JACK server outputs (use it with QJackCtl)

The initial goal of this project is to record from an old tape,
with an automatic split/pause feature when you are away.

Another goal is to have a litle tool ready to use with a minimum of configuration for simple stereo recording needs.

It manages natively silence by threshold and delay.
Silence event can be used to pause the record or to split files by closing the current record and opening a new one.

Optionaly QJackRcd is enable to post-process each file record at closure in background mode.
The command is a simple bash command.

Made with QTCreator, it Depends on jack, sndfile and qt5 libraries.

## AUTHOR

Olivier ROUITS <olivier.rouits@free.fr>

# BUILD

## COMPILE

`
qmake -config release

make clean

make
`

## DOXYGEN

you must install doxygen before making documentations:

`
make doxy
`

## EXECUTE

`
./qjackrcd
`


## INSTALL

`
sudo make install
`

# RELEASES

SNAPSHOT (testing)

1.2.0 (stable)
- FIX: conflicts between jack transport and record at launch feature in some cases.
- FIX: internal recording buffer size not adjusted (too long).
- Add recorded size indicators in status bar
- Add command line parser with all options
- Add a console mode only (no GUI) for automation purpose
- Add a man page with all options decribed.

1.1.2 (stable)
- Add an option to be in record state at launch time

1.1.1 (stable)
- QT5 5.3 back compatibility for debian Jessie
- FR translation fixes from trebmuh@tuxfamily.org

1.1.0 (stable)
- QT5 upgrade
- Output directory choice
- Jack options

1.0.6 (stable)
- German translation from Michael Dahms

1.0.5 (stable)
- FIX:No translation when installed from source in /usr/local
- Do not put Doxygen "docs" target mandatory
- Store/Try to restore last jack connections at launch
- Default delay is 3 seconds

1.0.4 (stable)
- Czech translation from "Pavel Fric"
- Doxygen documentation (docs subdir)
- Refactor build, build subdir with intermediate build files
- Refactor dist (renamed in publish) with only one tar.gz file with sources and last build and doc files
- Integration of SVN tags in Doxygen doc.

1.0.3 (testing)
- FIX: translations in /usr/share/qjackrcd/locale directory by default
- Desktop file from "speps"
- Italian translation from "speps"

1.0.2 (testing)
- FIX: on_timer slot warning message
- Auto connection to registered new jack ports (if no connections) to make recording ready to use when a jack player is launched
- Minimal documentation and code comments

1.0.1 (testing)
- FIX: IO file write outside RT jack call (Recorder thread + ringbuffer + mutex)
- FIX: Bad overlaped signal after several seconds due to ringbuffer full on eeepc when fs sync
- FR / EN translation activation
- Persist configuration in QT standard application settings
- Automatic jackd launch if not actived
- Jack shutdown listening (exit)
- Suppress pprocessor and integrate it into recorder class

1.0.0 (testing)
- First draft with IO calls inside jack callback (bad)
- Auto pause activation on DB threshold and delay
- Optional split files if pause
- Background post processing
- Jack transport listening (start/stop)
