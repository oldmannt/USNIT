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
    SFUsnitLogic.sharedInstance.handleCallback(type, data)
    return 1
}

public class SFUsnitLogic {
    static let sharedInstance = SFUsnitLogic()
    var observers_result:Array<UsnitLogicObserver>

    init() {

        observers_result = Array<UsnitLogicObserver>()
        let conf = load_conf()
        let strlangId = NSLocale.currentLocale().objectForKey(NSLocaleLanguageCode) as! String
        
        var nlangId = LANG_CH
        if strlangId.rangeOfString("zh")==nil {
            nlangId = LANG_ENG
        }
        
        UsnitInit(conf.file_path, nlangId, UsnitCallback);
    }
    
    private func load_conf()->(file_path:String, file_content:String){
        var json_str:String = ""
        let res_path = NSBundle.mainBundle().pathForResource("conf", ofType: "json")
        
        var doc_path:String = NSSearchPathForDirectoriesInDomains(.DocumentDirectory, .UserDomainMask, true)[0] as String
        doc_path.appendContentsOf("/conf.json")

        let fileManager = NSFileManager.defaultManager()
        if fileManager.fileExistsAtPath(doc_path) {
            g_logouts(LOG_FILE|LOG_CONSOLE, 3, "DOCUMENT CONF FILE AVAILABLE")
            do {
                json_str = try NSString(contentsOfFile: doc_path, usedEncoding: nil) as String
            }catch let error as NSError{
                // contents could not be loaded
                g_logouts(LOG_FILE|LOG_CONSOLE, LOG_ERROR, "SFUsnitLogic.init write failed \(error.userInfo) ")
                return (doc_path, json_str)
            }
        } else {
            g_logouts(LOG_FILE|LOG_CONSOLE, 3, "DOCUMENT CONF FILE NOT AVAILABLE")
            let apppath = NSBundle.mainBundle().pathForResource("conf", ofType: "json")
            do {
                json_str = try NSString(contentsOfFile: apppath!, usedEncoding: nil) as String
                
                do {
                    //try json_str.writeToURL(doc_conf_url, atomically: false, encoding: NSUTF8StringEncoding)
                    try json_str.writeToFile(doc_path, atomically: true, encoding: NSUTF8StringEncoding)
                    
                } catch let error as NSError{
                    // contents could not be loaded
                    g_logouts(LOG_FILE|LOG_CONSOLE, LOG_ERROR, "SFUsnitLogic.init write failed \(error.userInfo) ")
                    return (res_path!, json_str)
                }
            } catch let error as NSError{
                // contents could not be loaded
                g_logouts(LOG_FILE|LOG_CONSOLE, LOG_ERROR, "SFUsnitLogic.init \(error.userInfo) load app/conf.json failed")
            }
            
            
        }

        return (doc_path, json_str)
    }
    
    public func setUsnitInput(value : Float){
        UsnitSetInput(value)
    }
    
    public func getUsnitName(type : Int32)-> String?{
        let name = String.fromCString(UsnitGetUnitName(type))
        return name!
    }
    
    public func getUsnitResult(type : Int32)-> String?{
        let strMeterResult = String.fromCString(UsnitGetResult(type))
        return strMeterResult!
    }
    
    public func addObserver(observer:UsnitLogicObserver)->Bool{
        if observers_result.indexOf({$0 === observer}) != nil{
            return false
        }
        observers_result.append(observer)
        return true
    }
    
    public func removeObjserver(observer:UsnitLogicObserver)->Bool{
        let find = observers_result.indexOf({$0 === observer})
        if  find == nil {
            return false
        }
        
        observers_result.removeAtIndex(find!)
        return true
    }
    
    private func dispachResult(type: Int32, _ result:String){
        for observer in observers_result {
            observer.HandleResult(type, result)
        }
    }
    
    public func handleCallback(type: Int32, _ data: UnsafePointer<Void>)->Void{

        switch type {
        case CB_DOLLAR_RT, CB_RMB_RT, CB_RATEINFO:
            let result = (UnsafePointer<Int8>(data))
            self.dispachResult(type, String.fromCString(result)!)
            break;
            
            default:
                break;
        }

    }
}