#ifndef __VERSION_H__
#define __VERSION_H__


#define IB_PRODUCTION_VER	" l04"
#define IB_TEST_VER		" l01"
#define PB_PRODUCTION_VER	" P01"
#define SW_PART_NO		"0301-075385"

/*--------------IoT version--------------*/
#define IB_PRODUCTION_VER_IOT	0x01 //0x24		/*Interface Board Production_Version*/
#define PB_PRODUCTION_VER_IOT	0x01 //0x1F		/*Power Board Production_Version*/
/*---------------------------------------*/
/*
-----------------------------------Revision History---------------------------------------
******************************************************************************************
Date                    : 25/08/2025
Project Name		: NEO SERIES 8KG WITH KNOB 1200 (Silver sensorless bottom power board with rev i)
Software Specification  : EL-PROG 345
Software Version        : V4.1
Devloped By		: Anushree
Software Change         : 1. Load sensing step made time dependent (4mins timeout condition added) -- ECN:74161
			-- 2mins motor halt condition for any faulttaken care
			-- Post LS timeout, LS display will stop but it won't go 
		           to next step untill motor fault condition is exited.
			  2. Foam Sense new FS frequency (40.25kHz) with new FS drain out logic -- ECN: 72941
			  3. :AI issue due to time overflow resolved-- additional time was geting subtracted on restarting spin during foam sense logic
******************************************************************************************
Date                    : 21/01/2025
Project Name		: NEO SERIES 8KG WITH KNOB 1200 (Silver sensorless bottom power board with rev i)
Software Specification  : EL-PROG 345
Software Version        : V3.1
Devloped By		: SHUBHAM
Software Change         : 1. Foam sense delay eliminated.
			  2. Foam sense safety condition added.
*************************************************************
******************************************************************************************
Date                    : 13/12/2024
Project Name		: NEO SERIES 8KG WITH KNOB 1200 (Silver sensorless bottom power board with rev i)
Software Specification  : EL-PROG 345
Software Version        : V2.1
Devloped By		: SHUBHAM
Software Change         : 1. Latest knob changes . overlap issue fixed.
			  2. SF01 wool 800 & temp 40 passed
			  3. vf/md formula shift
******************************************************************************************
Date                    : 18/10/2024
Project Name		: NEO SERIES 8KG WITH KNOB (CEM 1 PCB)
Software Specification  : EL-PROG 345
Software Version        : V01.1
Devloped By		: SHUBHAM
Software Change         : 
			  1. lOAD SENSE FORMULA AS PER W/O KNOB MODEL 8012
			  2. VF/IF BIT IN DEBUG
			  3. KEY SENSING FAST 
******************************************************************************************
Date                    : 05/09/2024
Project Name		: NEO SERIES 8KG WITH KNOB (Silver sensorless bottom power board with rev i)
Software Specification  : EL-PROG 345
Software Version        : V04.1
Devloped By		: SHUBHAM
Software Change         : 
			  1. at Tubclean deselection reset prog parameter called after mix assignment.
			  2. ECO 60 & ECO 40 Ex rinse time made as per specs.
			  3 Safety condition added as per PR 78793
			   -0:00 field issue
			   - hpr in test mode SF02 LP stage 15 sec
******************************************************************************************
Date                    : 24/08/2024
Project Name		: NEO SERIES 8KG WITH KNOB (Silver sensorless bottom power board with rev i)
Software Specification  : EL-PROG 345
Software Version        : V03.1
Devloped By		: SHUBHAM
Software Change         : 1. Prewash on shirts missed - fixed!
			  2. Test mode mix program default changes.
			  3. SF02 tap error and heater on freq updated as per PR and spec update -- PR: 79016
******************************************************************************************
Date                    : 14/08/2024
Project Name		: NEO SERIES 8KG WITH KNOB (Silver sensorless bottom power board with rev i)
Software Specification  : EL-PROG 345
Software Version        : V02.1
Devloped By		: SHUBHAM
Software Change         : 1. HPRS error declaration delay changed from 50 to 90 seconds
******************************************************************************************
Date                    : 20/06/2024
Project Name		: NEO SERIES 8KG WITH KNOB (Silver sensorless bottom power board with rev i)
Software Specification  : EL-PROG 345
Software Version        : V01.2
Devloped By		: SHUBHAM
Software Change         : 1. Delay start changes.
			  2. warmsoak changes ;AI issue
			  3. steam led default to refresh,power steam,linen wash
			  4. prewash temp display skip in pause state updated
			  5. HTR changes
			  6. prewash shirt missed cases.
			  
******************************************************************************************
Date                    : 09/05/2024
Project Name		: NEO SERIES 8KG WITH KNOB (Silver sensorless bottom power board with rev i)
Software Specification  : EL-PROG 345
Software Version        : V01.1
Devloped By		: Anushree
Software Change         : 1. Max rpm limit made 1200
			  2. Program with default option selected, option led was not glowing --updated in prog selector to keep by default option led glowing (uioption_on)
			  3. Eco40/60 nd additive rinse FS2 with 11 mins step time and program time updated
			  4. Load sensing formula updated
******************************************************************************************
*/

#endif
