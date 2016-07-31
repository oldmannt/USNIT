# USNIT

this project run on android and ios (web will be done, if have time), UI in native and common logic in c++  

mini C++ ui bridge, adapt native ui to c++, use same c++ code to build ui programatically for ios and android

build apk for public: `make android`
build ios for public: need archive in xcode 

![image](https://github.com/oldmannt/USNIT/blob/master/data/screen/ios-github.png) 
![image](https://github.com/oldmannt/USNIT/blob/master/data/screen/android-github.jpg) 


generate ui by cofig file, modify `/data/ui.json` and `/data/uilogic.json`.  
add some `setting ui` wll suppert user costum ui, but I have no time to do it, my next project can't wait

In ios, support user adds any new unit by modifying JSON files in document directory. Make sure you understand before modifying JSON
uilogic.json
	A unit
     {"name":"jin", "us":false, "multiply":0.002},
	A supper type
	    {"name":"press", "base_unit":"pa", "units":[
       {"name":"pa", "us":false,"func":"", "multiply":1},
       {"name":"kpa", "us":false,"func":"", "multiply":0.001},
       {"name":"psi", "us":true,"func":"", "multiply":0.000145},
       {"name":"psf", "us":true,"func":"", "multiply":0.0208854}
    ]}

Language.json
	  "pa":{"chs":"帕", "eng":"Pa"},
    "kpa":{"chs":"千帕", "eng":"kPa"},
    "psi":{"chs":"磅英寸", "eng":"psi"},
    "psf":{"chs":"磅英尺", "eng":"psf"}
    "jin":{"chs":"斤", "eng":"jin"},
    "liang":{"chs":"两", "eng":"liang"},
    
    
In android, need to modify mainactivity.xml, and need to republish

