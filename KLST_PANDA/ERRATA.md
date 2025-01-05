# KLANGSTROM / KLST_PANDA / ERRATA

# v2.0

- via short circuits display trace

# v1.0

- @module(battery) SS14 diode which i added as polarity protection prevents the battery from charging. @fix(remove diode and replaced with solder bridge )
- outer PCB layers ( top and bottom ) have no copper pour ( they got lost when uploading files a second time ). @fix(take extra care to add pours next iteration)
- @module(on-board_microphone) missing decoupling capacitors at right ( i.e on-off switch side ) microphone
- @module(on-board_microphone) microphone is rotate 180°
- @module(audio_codec) some units must receive 3.3V instead of 1.8V