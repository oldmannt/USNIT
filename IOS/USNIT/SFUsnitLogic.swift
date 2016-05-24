//
//  SFUsnitLogic.swift
//  USNIT
//
//  Created by dyno on 4/9/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation

public protocol UsnitLogicObserver:AnyObject {
    func HandleResult(type: USNUsnitEventType, _ result:String) -> Bool
}

public class UsnitEventHander: USNUsnitEventGen{
    @objc public func callback(id: USNUsnitEventType, data: String) -> Bool{
        return SFUsnitLogic.sharedInstance.handleCallback(id, data)
    }
}

public class SFUsnitLogic {
    static let sharedInstance = SFUsnitLogic()
    var observers_result:Array<UsnitLogicObserver>
    var event_hander:UsnitEventHander
    init() {

        observers_result = Array<UsnitLogicObserver>()
        event_hander = UsnitEventHander()
        let conf = load_conf()
        let strlangId = NSLocale.currentLocale().objectForKey(NSLocaleLanguageCode) as! String
        
        var lang_type = USNLangType.LANGCH
        if strlangId.rangeOfString("zh")==nil {
            lang_type = USNLangType.LANGENG
        }
        
        USNUsnitGen.instance()!.initialize(conf.file_path, lang: lang_type, callback: event_hander)
    }
    
    private func load_conf()->(file_path:String, file_content:String){
        var json_str:String = ""
        let res_path = NSBundle.mainBundle().pathForResource("conf", ofType: "json")
        
        var doc_path:String = NSSearchPathForDirectoriesInDomains(.DocumentDirectory, .UserDomainMask, true)[0] as String
        doc_path.appendContentsOf("/conf.json")

        let fileManager = NSFileManager.defaultManager()
        if fileManager.fileExistsAtPath(doc_path) {
            GBLogGen.instance()?.log(GBLogGenLOGCONSOLE|GBLogGenLOGFILE, lev: GBLogGenLOGINFO, msg: "DOCUMENT CONF FILE AVAILABLE")
            do {
                json_str = try NSString(contentsOfFile: doc_path, usedEncoding: nil) as String
            }catch let error as NSError{
                // contents could not be loaded
                GBLogGen.instance()?.log(GBLogGenLOGCONSOLE|GBLogGenLOGFILE, lev: GBLogGenLOGINFO,
                                         msg: "SFUsnitLogic.init write failed \(error.userInfo) ")
                return (doc_path, json_str)
            }
        } else {
            GBLogGen.instance()?.log(GBLogGenLOGCONSOLE|GBLogGenLOGFILE, lev: GBLogGenLOGINFO, msg: "document conf don't exist")
            do {
                json_str = try NSString(contentsOfFile: res_path!, usedEncoding: nil) as String
                
                do {
                    //try json_str.writeToURL(doc_conf_url, atomically: false, encoding: NSUTF8StringEncoding)
                    try json_str.writeToFile(doc_path, atomically: true, encoding: NSUTF8StringEncoding)
                    GBLogGen.instance()?.log(GBLogGenLOGCONSOLE|GBLogGenLOGFILE, lev: GBLogGenLOGINFO, msg: "document conf been writed")
                    
                } catch let error as NSError{
                    // contents could not be loaded
                    GBLogGen.instance()?.log(GBLogGenLOGCONSOLE|GBLogGenLOGFILE, lev: GBLogGenLOGERROR,
                                             msg: "SFUsnitLogic.init write failed \(error.userInfo) ")
                    return (res_path!, json_str)
                }
            } catch let error as NSError{
                // contents could not be loaded
                GBLogGen.instance()?.log(GBLogGenLOGCONSOLE|GBLogGenLOGFILE, lev: GBLogGenLOGERROR,
                                         msg: "SFUsnitLogic.init \(error.userInfo) load app/conf.json failed")
            }
            
            
        }

        return (doc_path, json_str)
    }
    
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

    }
}