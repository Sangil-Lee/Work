## lakeshore

EPICS support for the Lakeshore 336/350 series controllers 

[Lakeshore 336](http://www.lakeshore.com/products/cryogenic-temperature-controllers/model-336/Pages/Overview.aspx)

[Lakeshore 350](http://www.lakeshore.com/products/cryogenic-temperature-controllers/model-350/Pages/Overview.aspx)

The 336 controller is almost identical to the 350, and the same
software can be used for both. Where the settings do differ this 
module provides generic menu options that apply to both controllers.

The module can also support the extra inputs supplied by the 3062
option card. 

The EPICS support is entirely based on [Asyn](http://www.aps.anl.gov/epics/modules/soft/asyn/) and [Stream Device](http://epics.web.psi.ch/software/streamdevice/).

The module provides support for:

* Reading temperature and raw inputs
* Support for reading 3062 option card inputs
* Setting up channel output parameters such as setpoint and ramp rate
* Reading channel output parameters
* Support for 4 PID loop outputs, including 2 analog outputs
* Support for setting ZONE parameters for all 4 output channels
* Channel access put_callback support when setting parameters
* Channel access put_callback support for setting the temperature (using a busy record and a tolerance window).
* Support for setting and reading input sensor descriptions
* Support for reading sensor and heater status and alarm conditions
* High level logic to provide a lakeshore summary alarm, and control of which sensor is used for the summary alarm.
* Logic that provides a master/slave relationship between two outputs, with a setpoint dependent offset table.
* Set of CS-Studio BOY screens

### Release history

1-0: 
Original release.

1-1: 
Add support for setting and reading input sensor descriptions (and related GUI OPI changes). 
Factor out common records for input sensors into new template file (lakeshore_input.template). 

1-2:
Add support for put_callback on the temperature setpoint. A temperature window is used, and a timer. The temperature must be within the window, and the timer must have expired, before the callback is done. The database automatically detects if the input changes for a particular channel.
Also add associated GUI support to the OPI files.

1-3:
* Removed timer logic for callback. It was complicated and there may be a race condition. Now the callback just uses the tolerance window.
* Use SETP_S and not SETP for the tolerance check.
* Archive a lot of additional parameters.
* Add lakeshore_link_output.template to use when linking together two outputs in a master/slave relationship, with a temperature dependent offset table.
* Read sensor status, type, curve setting and alarm conditions.
* Read heater status for output 1 and 2. Cache the value and use in high level alarm logic.
* Provide an alarm summary record for the input parameters (software limits, alarm status and input sensor status).
* Add records to control polling for each input, for the input specific data. This allows easy disable of a channel.
* Add OPI support for all the above.

1-4:
* Add support for setting all the outputs at once, and all the tolerance windows at once.
* Add support for mapping any of the sensor records to a high level user friendly 'sample temperature' PV name.
* Add OPI support for the above.


### Building lakeshore

The lakeshore336 directory can be build as a standard EPICS support
module. You may have to edit the files in the configure directory
for your local site.

### Building an IOC using lakeshore

Include the lakeshore support in the IOC configure/RELEASE file, and 
either instantiate the EPICS database in the IOC startup script or in a
substitutions file. For example:
```
   file lakeshore336.template
   {
      pattern {P, PORT, ADDR, TEMPSCAN, SCAN}
              {BL9:SE:LS, L0, 0, 1, 5}
   }
```
where the macros are:
```
   P - Base PV name
   PORT - Asyn port name
   ADDR - Asyn port addr
   TEMPSCAN - Scan frequency for the temperatures and raw inputs (eg. 1 second)
   SCAN - Scan frequency for the auxiliary parameters such as output ramp rate (eg. 5 seconds)
```
To use the master/slave relationship:
```
   file lakeshore_link_output.template
   {
      pattern {P, MASTER, SLAVE}
	      {BL9:SE:LS, 2, 1}
   }
```

There are no libraries to link to apart from the usual Asyn and StreamDevice libraries. 

There is an example IOC provided with the module.





