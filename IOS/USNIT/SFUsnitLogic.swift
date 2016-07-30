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

       
        let language_path = load_conf("language", ext:"json")
        GBLanguageStoreGen.instance()?.initialize(language_path)
        
        let conf = load_conf("user", ext: "json")
        GBUserConfigGen.instance()?.initialize(conf)
        
        let strlangId = NSLocale.currentLocale().objectForKey(NSLocaleLanguageCode) as! String
        if strlangId.rangeOfString("zh")==nil {
            GBLanguageStoreGen.instance()?.setLanguage(GBLangType.LANGENG)
        }
        else{
            GBLanguageStoreGen.instance()?.setLanguage(GBLangType.LANGCHS)
        }
        
        let uilogic_path = load_conf("uilogic", ext: "json")
        USNUilogicGen.instance()?.initialize(uilogic_path)
        USNUilogicGen.instance()?.buildUi()
        //USNUsnitGen.instance()!.initialize(uilogic_path, lang: 0, callback: nil)
    }
    
    private func load_conf(file_name:String, ext:String)->String{
        var json_str:String = ""
        let res_path = NSBundle.mainBundle().pathForResource(file_name, ofType: ext)
        
        let doc_path:String = NSSearchPathForDirectoriesInDomains(.DocumentDirectory, .UserDomainMask, true)[0] as String
        
        var conf_file = doc_path
        if let version = NSBundle.mainBundle().infoDictionary?["CFBundleVersion"] as? String {
            conf_file.appendContentsOf("/\(file_name).\(version).\(ext)")
        }
        else{
            conf_file.appendContentsOf("/\(file_name).1.11.\(ext)")
        }
        
        let fileManager = NSFileManager.defaultManager()
        if fileManager.fileExistsAtPath(conf_file) {
            do {
                json_str = try NSString(contentsOfFile: conf_file, usedEncoding: nil) as String
                return conf_file
            }catch let error as NSError{
                // contents could not be loaded
                GBLogGen.instance()?.logerrf("SFUsnitLogic.load_conf read \(conf_file) failed \(error.userInfo) ")
                return res_path!
            }
        } else {
            deleteFilesInPath(doc_path, range: file_name)
            do {
                json_str = try NSString(contentsOfFile: res_path!, usedEncoding: nil) as String
                
                do {
                    try json_str.writeToFile(conf_file, atomically: true, encoding: NSUTF8StringEncoding)
                    
                } catch let error as NSError{
                    // contents could not be loaded
                    GBLogGen.instance()?.logerrf("SFUsnitLogic.load_conf write \(conf_file) failed \(error.userInfo) ")
                    return conf_file
                }
            } catch let error as NSError{
                // contents could not be loaded
                GBLogGen.instance()?.logerrf("SFUsnitLogic.load_conf read \(res_path) \(error.userInfo) load app/conf.json failed")
                return res_path!
            }
            
            
        }

        return res_path!
    }
    
    private func deleteFilesInPath(path:String, range:String) {
        let filemanager:NSFileManager = NSFileManager()
        var is_dir:ObjCBool = false
        let files:NSDirectoryEnumerator? = filemanager.enumeratorAtPath(path)
        while let file = files?.nextObject() as? String {
            
            let file_path = path+"/\(file)"
            
            filemanager.fileExistsAtPath(file_path, isDirectory: &is_dir)
            //print(is_dir ? "is directory" : "is a file")
            if file.rangeOfString(range) != nil{
                do {
                    try filemanager.removeItemAtPath(file_path);
                }catch let error as NSError{
                    print("SFUsnitLogic.init write failed \(error.userInfo) ")
                }
            }
        }
        
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