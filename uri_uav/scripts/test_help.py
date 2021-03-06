#!/usr/bin/env python


print ""
print ""
print "________________________________________________________________________________"
print "The following arguments are mandatory to launch this test."
print ""
print	"test"
print "selects the test to be executed. Possible values are:"
print	"- takeoff_hover_land"
print	"- takeoff_goto_land"
print	"- takeoff_circling_land"
print ""
print	"gazebo_world"
print "selects the gazebo world that is loaded from the package uri_gazebo. Possible values are:"
print	"- beach"
print	"- bridge"
print	"- nogazebo"
print	"- shore"
print ""
print	"gazebo_interface"
print "selects whether a gazebo interface is needed. Possible values are:"
print	"- yes"
print	"- no"
print ""
print	"fcu_url"
print "selects where mavros looks for the floght controller. Possible values are:"
print	"- udp://:14550@127.0.0.1:14550  for gazebo"
print	"- /dev/ttyACM0:57600            usb connected iris"
print	"- /dev/ttyUSB0:57600            radio connected iris"
print "________________________________________________________________________________"
print ""
print ""
	
print "Press ctrl+C to terminate..."
	
	