# Network notes


## ip UNKNOWN

Interfaces sometimes report his state as `UNKNOWN`, this could be a failure from the driver to report
all the needed bits. 
Each interface has at least to different kinds of states:

* Administrative state
    * Describes if the interface is enabled
    * It's either up/down and it is controlled my network software or tools like `ip`
* Operational state
    * Describes whether the interface _works_
    * Just because is enabled, it does not mean that it can transfer data, ethernet cable needs to be plugged
    * In the case of WiFi it needs to be asociated with an AP, for both it means a carrier
    * If the carrier is not there the state would be `LOWER_UP`

Then the UNKNOWN is not the administrative state, it's the operational state which comes from
the driver system itself, it shows a set of values provided by the driver to the kernel