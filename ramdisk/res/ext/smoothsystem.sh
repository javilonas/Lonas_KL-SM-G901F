#!/system/bin/sh
#

while [ 1 ]; do

for n in 1 2
do

  USER_LAUNCHER1="com.gau.go.launcherex"				# Change this to your launcher app
  USER_LAUNCHER2="com.android.launcher2"				# Change this to your launcher app
  USER_LAUNCHER3="com.teslacoilsw.launcher"				# Change this to your launcher app
  USER_LAUNCHER4="com.anddoes.launcher" 				# Change this to your launcher app
  USER_LAUNCHER5="com.sec.android.app.launcher"		# Change this to your launcher app
  USER_LAUNCHER6="com.android.launcher"				# Change this to your launcher app
  USER_LAUNCHER7="com.teslacoilsw.launcher" 				# Change this to your launcher app
  USER_LAUNCHER8="com.teslacoilsw.launcher.prime"		# Change this to your launcher app
  USER_LAUNCHER9="com.cyanogenmod.trebuchet"		# Change this to your launcher app
  USER_LAUNCHER10="org.adw.launcher"		# Change this to your launcher app

  NUMBER_OF_CHECKS=60					# Total number of rechecks before ending 1st loop
  SLEEP_TIME=3						# Number of seconds between rechecking processes
  PROCESSES_TOTAL=29				        # Must be edited to match the number of processes to be checked
  DEBUG_ECHO=0						# Debug on/off

  CHECK_COUNT=0						# Don't edit
  P_CHECK=0						    # Don't edit
  CHECK_OK=0						# Unused

  PROCESS_1=0; PROCESS_2=0; PROCESS_3=0; PROCESS_4=0; PROCESS_5=0; PROCESS_6=0;
  PROCESS_7=0; PROCESS_8=0; PROCESS_9=0; PROCESS_10=0; PROCESS_11=0; PROCESS_12=0;
  PROCESS_13=0; PROCESS_14=0; PROCESS_15=0; PROCESS_16=0; PROCESS_17=0; PROCESS_18=0;
  PROCESS_19=0; PROCESS_20=0; PROCESS_21=0; PROCESS_22=0; PROCESS_23=0; PROCESS_24=0;
  PROCESS_25=0; PROCESS_26=0; PROCESS_27=0; PROCESS_28=0; PROCESS_29=0;

  if [ $n -eq "1" ]; then
    if [ $DEBUG_ECHO -eq "1" ]; then
      echo ""
      echo "LST Debug: $(date)"
      echo "LST Debug: Initiate"
    fi;

    # Pause and then loop until kswapd0 is found, which should be instant anyway
    sleep 1
    SWAP_SLEEP_TIME=3; SWAP_NUMBER_OF_CHECKS=30; SWAP_CHECK_COUNT=0; SWAP_CHECK_COUNT_OK=0; while [ $SWAP_CHECK_COUNT -lt $SWAP_NUMBER_OF_CHECKS ]; do if [ `pidof kswapd0` ]; then renice 19 `pidof kswapd0`; SWAP_CHECK_COUNT=$SWAP_NUMBER_OF_CHECKS; SWAP_CHECK_COUNT_OK=1; else sleep $SWAP_SLEEP_TIME; fi; SWAP_CHECK_COUNT=`expr $SWAP_CHECK_COUNT + 1`; done; if [ $SWAP_CHECK_COUNT_OK -lt 1 ]; then echo "LST Debug: 'kswapd0' expired after `expr $SWAP_CHECK_COUNT \* $SWAP_SLEEP_TIME` seconds"; fi;

    if [ $DEBUG_ECHO -eq "1" ]; then
      echo "LST Debug: $(date)";
      echo "LST Debug: kswapd0 found";
    fi;
  fi;

  # Check briefly one more time
  if [ $n -eq "2" ]; then
    if [ $DEBUG_ECHO -eq "1" ]; then
      echo "LST Debug: 2nd loop"
    fi;
    NUMBER_OF_CHECKS=6					# Editing not recommended
    SLEEP_TIME=5					# Editing not recommended
  fi;

  while [ $CHECK_COUNT -lt $NUMBER_OF_CHECKS ];
  do
    # Resident system apps
    if [ $PROCESS_1 -eq "0" ]; then PNAME="com.android.phone"; NICELEVEL=-12; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_1=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_2 -eq "0" ]; then PNAME="com.android.systemui"; NICELEVEL=-10; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_2=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_3 -eq "0" ]; then PNAME="com.android.settings"; NICELEVEL=-10; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_3=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_4 -eq "0" ]; then PNAME="$USER_LAUNCHER1"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_4=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_5 -eq "0" ]; then PNAME="com.android.smspush"; NICELEVEL=-8; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_5=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_6 -eq "0" ]; then PNAME="android.process.acore"; NICELEVEL=-3; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_6=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_7 -eq "0" ]; then PNAME="android.process.media"; NICELEVEL=-1; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_7=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_8 -eq "0" ]; then PNAME="com.android.inputmethod.latin"; NICELEVEL=-4; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_8=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_9 -eq "0" ]; then PNAME="$USER_LAUNCHER2"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_9=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER2)/oom_adj; fi; fi;
    if [ $PROCESS_10 -eq "0" ]; then PNAME="$USER_LAUNCHER3"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_10=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER3)/oom_adj; fi; fi;
    if [ $PROCESS_11 -eq "0" ]; then PNAME="$USER_LAUNCHER4"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_11=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER4)/oom_adj; fi; fi;
    if [ $PROCESS_12 -eq "0" ]; then PNAME="$USER_LAUNCHER5"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_12=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER5)/oom_adj; fi; fi;
    if [ $PROCESS_13 -eq "0" ]; then PNAME="$USER_LAUNCHER1"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_13=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER1)/oom_adj; fi; fi;
    if [ $PROCESS_25 -eq "0" ]; then PNAME="$USER_LAUNCHER6"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_25=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER6)/oom_adj; fi; fi;
    if [ $PROCESS_26 -eq "0" ]; then PNAME="$USER_LAUNCHER7"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_26=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER7)/oom_adj; fi; fi;
    if [ $PROCESS_27 -eq "0" ]; then PNAME="$USER_LAUNCHER8"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_27=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER8)/oom_adj; fi; fi;
    if [ $PROCESS_28 -eq "0" ]; then PNAME="$USER_LAUNCHER9"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_28=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER9)/oom_adj; fi; fi;	
    if [ $PROCESS_29 -eq "0" ]; then PNAME="$USER_LAUNCHER10"; NICELEVEL=-14; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_29=1; P_CHECK=`expr $P_CHECK + 1`; echo "-17" > /proc/$(pidof $USER_LAUNCHER10)/oom_adj; fi; fi;	

    #####################
    # Other system apps #
    #####################

    if [ $PROCESS_14 -eq "0" ]; then PNAME="com.sec.android.app.dialertab"; NICELEVEL=-19; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_14=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_15 -eq "0" ]; then PNAME="com.sec.android.app.samsungapps"; NICELEVEL=-19; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_15=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_16 -eq "0" ]; then PNAME="com.sec.android.app.controlpanel"; NICELEVEL=-18; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_16=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_17 -eq "0" ]; then PNAME="com.sec.android.app.camera"; NICELEVEL=-6; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_17=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;

    ##############
    # Other apps #
    ##############

    if [ $PROCESS_18 -eq "0" ]; then PNAME="com.android.tsm_parts"; NICELEVEL=-15; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_18=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_19 -eq "0" ]; then PNAME="os.tools.scriptmanager"; NICELEVEL=-15; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_19=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_20 -eq "0" ]; then PNAME="com.mobint.locker"; NICELEVEL=-15; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_20=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_21 -eq "0" ]; then PNAME="com.android.browser"; NICELEVEL=-9; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_21=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_22 -eq "0" ]; then PNAME="com.google.android.youtube"; NICELEVEL=-6; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_22=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_23 -eq "0" ]; then PNAME="com.google.android.apps.maps"; NICELEVEL=-6; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_23=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;
    if [ $PROCESS_24 -eq "0" ]; then PNAME="pidof kondemand/0"; NICELEVEL=19; if [ `pidof $PNAME` ]; then renice $NICELEVEL `pidof $PNAME`; PROCESS_24=1; P_CHECK=`expr $P_CHECK + 1`; fi; fi;

	
    # If all processes are done, loop can finish early
    if [ $P_CHECK -ge $PROCESSES_TOTAL ]; then CHECK_COUNT=$NUMBER_OF_CHECKS; else sleep $SLEEP_TIME; fi;

    CHECK_COUNT=`expr $CHECK_COUNT + 1`;
  done

  if [ $DEBUG_ECHO -eq "1" ]; then
    echo "LST Debug: $(date)"
    if [ $PROCESS_1 -eq "0" ]; then echo "LST Debug: PROCESS_1 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_2 -eq "0" ]; then echo "LST Debug: PROCESS_2 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_3 -eq "0" ]; then echo "LST Debug: PROCESS_3 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_4 -eq "0" ]; then echo "LST Debug: PROCESS_4 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_5 -eq "0" ]; then echo "LST Debug: PROCESS_5 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_6 -eq "0" ]; then echo "LST Debug: PROCESS_6 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_7 -eq "0" ]; then echo "LST Debug: PROCESS_7 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_8 -eq "0" ]; then echo "LST Debug: PROCESS_8 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_9 -eq "0" ]; then echo "LST Debug: PROCESS_9 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_10 -eq "0" ]; then echo "LST Debug: PROCESS_10 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_11 -eq "0" ]; then echo "LST Debug: PROCESS_11 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_12 -eq "0" ]; then echo "LST Debug: PROCESS_12 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_13 -eq "0" ]; then echo "LST Debug: PROCESS_13 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_14 -eq "0" ]; then echo "LST Debug: PROCESS_14 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_15 -eq "0" ]; then echo "LST Debug: PROCESS_15 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_16 -eq "0" ]; then echo "LST Debug: PROCESS_16 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_17 -eq "0" ]; then echo "LST Debug: PROCESS_17 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_18 -eq "0" ]; then echo "LST Debug: PROCESS_18 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_19 -eq "0" ]; then echo "LST Debug: PROCESS_19 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_20 -eq "0" ]; then echo "LST Debug: PROCESS_20 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_21 -eq "0" ]; then echo "LST Debug: PROCESS_21 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_22 -eq "0" ]; then echo "LST Debug: PROCESS_22 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_23 -eq "0" ]; then echo "LST Debug: PROCESS_23 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_24 -eq "0" ]; then echo "LST Debug: PROCESS_24 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_25 -eq "0" ]; then echo "LST Debug: PROCESS_20 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_26 -eq "0" ]; then echo "LST Debug: PROCESS_21 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_27 -eq "0" ]; then echo "LST Debug: PROCESS_22 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_28 -eq "0" ]; then echo "LST Debug: PROCESS_23 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;
    if [ $PROCESS_29 -eq "0" ]; then echo "LST Debug: PROCESS_24 expired after `expr $CHECK_COUNT \* $SLEEP_TIME` seconds"; fi;

    echo "LST Debug: Checking complete"
  fi;

done

if [ $DEBUG_ECHO -eq "1" ]; then
  echo "LST Debug: Done"
  echo ""
fi;

sleep 7200
done
