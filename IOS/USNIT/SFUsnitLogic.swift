//
//  SFUsnitLogic.swift
//  USNIT
//
//  Created by dyno on 4/9/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation

public protocol UsnitLogicObserver:AnyObject {
    func HandleResult(type: Int32, _ result:String) -> Bool
}

func UsnitCallback(type: Int32, _ data: UnsafePointer<Void>)->Int32 {
    SFUsnitLogic.sharedInstance.HandleCallback(type, data)
    return 1
}

public class SFUsnitLogic {
    static let sharedInstance = SFUsnitLogic()
    var observers_result:Array<UsnitLogicObserver>

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
        
        UsnitInit(json_str, nlangId, UsnitCallback);
        observers_result = Array<UsnitLogicObserver>()
    }
    
    public func SetUsnitInput(value : Float){
        UsnitSetInput(value)
    }
    
    public func GetUsnitResult(type : Int32)-> String?{
        let strMeterResult = String.fromCString(UsnitGetResult(type))
        return strMeterResult!
    }
    
    public func AddObserver(observer:UsnitLogicObserver)->Bool{
        if observers_result.indexOf({$0 === observer}) != nil{
            return false
        }
        observers_result.append(observer)
        return true
    }
    
    public func RemoveObjserver(observer:UsnitLogicObserver)->Bool{
        let find = observers_result.indexOf({$0 === observer})
        if  find == nil {
            return false
        }
        
        observers_result.removeAtIndex(find!)
        return true
    }
    
    private func DispachResult(type: Int32, _ result:String){
        for observer in observers_result {
            observer.HandleResult(type, result)
        }
    }
    
    public func HandleCallback(type: Int32, _ data: UnsafePointer<Void>)->Void{

        switch type {
        case CB_DOLLAR_RT, CB_RMB_RT, CB_RATEINFO:
            let result = (UnsafePointer<Int8>(data))
            self.DispachResult(type, String.fromCString(result)!)
            break;
            
            default:
                break;
        }

    }
}