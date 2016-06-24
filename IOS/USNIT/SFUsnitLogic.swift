//
//  SFUsnitLogic.swift
//  USNIT
//
//  Created by dyno on 4/9/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation

/*
public protocol UsnitLogicObserver:AnyObject {
    func HandleResult(type: USNUsnitEventType, _ result:String) -> Bool
}

public class UsnitEventHander: USNUsnitEventGen{
    @objc public func callback(id: USNUsnitEventType, data: String) -> Bool{
        return SFUsnitLogic.sharedInstance.handleCallback(id, data)
    }
}*/

public class SFUsnitLogic {
    static let sharedInstance = SFUsnitLogic()

    init() {

       
        let language_path = NSBundle.mainBundle().pathForResource("language", ofType: "json")
        GBLanguageStoreGen.instance()?.initialize(language_path!)
        
        let conf = load_conf()
        GBUserConfigGen.instance()?.initialize(conf)
        
        let strlangId = NSLocale.currentLocale().objectForKey(NSLocaleLanguageCode) as! String
        if strlangId.rangeOfString("zh")==nil {
            GBLanguageStoreGen.instance()?.setLanguage(GBLangType.LANGENG)
        }
        else{
            GBLanguageStoreGen.instance()?.setLanguage(GBLangType.LANGCHS)
        }
        
        let uilogic_path = NSBundle.mainBundle().pathForResource("uilogic", ofType: "json")
        USNUilogicGen.instance()?.initialize(uilogic_path!)
        //USNUsnitGen.instance()!.initialize(uilogic_path, lang: 0, callback: nil)
    }
    
    private func load_conf()->String{
        var json_str:String = ""
        let res_path = NSBundle.mainBundle().pathForResource("user", ofType: "json")
        
        var doc_path:String = NSSearchPathForDirectoriesInDomains(.DocumentDirectory, .UserDomainMask, true)[0] as String
        if let version = NSBundle.mainBundle().infoDictionary?["CFBundleVersion"] as? String {
            doc_path.appendContentsOf("/user.\(version).json")
        }
        else{
            doc_path.appendContentsOf("/1.5.json")
        }
        
        let fileManager = NSFileManager.defaultManager()
        if fileManager.fileExistsAtPath(doc_path) {
            do {
                json_str = try NSString(contentsOfFile: doc_path, usedEncoding: nil) as String
            }catch let error as NSError{
                // contents could not be loaded
                GBLogGen.instance()?.logerrf("SFUsnitLogic.init write failed \(error.userInfo) ")
                return res_path!
            }
        } else {
            do {
                json_str = try NSString(contentsOfFile: res_path!, usedEncoding: nil) as String
                
                do {
                    try json_str.writeToFile(doc_path, atomically: true, encoding: NSUTF8StringEncoding)
                    
                } catch let error as NSError{
                    // contents could not be loaded
                    GBLogGen.instance()?.logerrf("SFUsnitLogic.init write failed \(error.userInfo) ")
                    return res_path!
                }
            } catch let error as NSError{
                // contents could not be loaded
                GBLogGen.instance()?.logerrf("SFUsnitLogic.init \(error.userInfo) load app/conf.json failed")
                return res_path!
            }
            
            
        }

        return doc_path
    }
    
    /*
    public func setUsnitInput(value : Float){
        USNUsnitGen.instance()?.setInput(value)
    }
    
    public func getUsnitName(type : USNUsnitType)-> String?{
        return USNUsnitGen.instance()?.getUnitName(type)
    }
    
    public func getUsnitResult(type : USNUsnitType)-> String?{
        return USNUsnitGen.instance()?.getResult(type)
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
    
    private func dispachResult(type: USNUsnitEventType, _ result:String){
        for observer in observers_result {
            observer.HandleResult(type, result)
        }
    }
    
    public func handleCallback(type: USNUsnitEventType, _ data: UnsafePointer<Void>)->Bool{
        switch type {
        case USNUsnitEventType.CBDOLLARRT, USNUsnitEventType.CBRMBRT, USNUsnitEventType.CBRATEINFO:
            let result = (UnsafePointer<Int8>(data))
            self.dispachResult(type, String.fromCString(result)!)
            break;
            
            default:
                break;
        }
        
        return true;

    }*/
}