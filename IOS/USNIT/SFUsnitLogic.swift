//
//  SFUsnitLogic.swift
//  USNIT
//
//  Created by dyno on 4/9/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation

public class SFUsnitLogic {
    static let sharedInstance = SFUsnitLogic()
    
    init() {

        let path = NSBundle.mainBundle().pathForResource("conf", ofType: "json")
        var json_str = ""
        
        do {
            json_str = try NSString(contentsOfFile: path!, usedEncoding: nil) as String
        } catch {
            // contents could not be loaded
        }
        
        let strlangId = NSLocale.currentLocale().objectForKey(NSLocaleLanguageCode) as! String
        
        var nlangId = LANG_CH
        if strlangId.rangeOfString("zh")==nil {
            nlangId = LANG_ENG
        }
        
        UsnitInit(json_str, nlangId);

    }
    
    public func SetUsnitInput(value : Float, long_type:Int32 = TYPE_METER, mass_type:Int32 = TYPE_GRAM, square_type:Int32 = TYPE_SQM){
        
    }
    
    public func GetUsnitResult(type : Int32)-> String{
        let strMeterResult = String.fromCString(UsnitGetResult(0))
        return strMeterResult!
    }
}