//
//  FirstViewController.swift
//  USNIT
//
//  Created by dyno on 3/26/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation
import UIKit


class FirstViewController: UIViewController,UsnitLogicObserver {

    @IBOutlet weak var scView: UIScrollView!
    @IBOutlet weak var lblMeter: UILabel!
    @IBOutlet weak var lblCMeter: UILabel!
    @IBOutlet weak var lblKMeter: UILabel!
    @IBOutlet weak var lblFeet: UILabel!
    @IBOutlet weak var lblInch: UILabel!
    @IBOutlet weak var lblMile: UILabel!
    @IBOutlet weak var lblYard: UILabel!
    @IBOutlet weak var lblSqm: UILabel!
    @IBOutlet weak var lblSqcm: UILabel!
    @IBOutlet weak var lblSqfeet: UILabel!
    @IBOutlet weak var lblLiter: UILabel!
    @IBOutlet weak var lblMliter: UILabel!
    @IBOutlet weak var lblGal: UILabel!
    @IBOutlet weak var lblGram: UILabel!
    @IBOutlet weak var lblKGram: UILabel!
    @IBOutlet weak var lblPound: UILabel!
    @IBOutlet weak var lblOz: UILabel!
    @IBOutlet weak var lblCelsius: UILabel!
    @IBOutlet weak var lblFahrenheit: UILabel!
    @IBOutlet weak var lblSqinch: UILabel!
    @IBOutlet weak var lblDollar: UILabel!
    @IBOutlet weak var lblRmb: UILabel!
    
    @IBOutlet weak var lblUcm: UILabel!
    @IBOutlet weak var lblUm: UILabel!
    @IBOutlet weak var lblUkm: UILabel!
    @IBOutlet weak var lblUft: UILabel!
    @IBOutlet weak var lblUinch: UILabel!
    @IBOutlet weak var lblUmile: UILabel!
    @IBOutlet weak var lblUyard: UILabel!
    @IBOutlet weak var lblUsqm: UILabel!
    @IBOutlet weak var lblUsqcm: UILabel!
    @IBOutlet weak var lblUsqfeet: UILabel!
    @IBOutlet weak var lblUsqinch: UILabel!
    @IBOutlet weak var lblUliter: UILabel!
    @IBOutlet weak var lblUmliter: UILabel!
    @IBOutlet weak var lblUgal: UILabel!
    @IBOutlet weak var lblUgram: UILabel!
    @IBOutlet weak var lblUkgram: UILabel!
    @IBOutlet weak var lblUlb: UILabel!
    @IBOutlet weak var lblUoz: UILabel!
    @IBOutlet weak var lblUcelsius: UILabel!
    @IBOutlet weak var lblUfahrenheit: UILabel!
    @IBOutlet weak var lblUdollar: UILabel!
    @IBOutlet weak var lblUrmb: UILabel!
    @IBOutlet weak var lblExchange: UILabel!
    
    @IBOutlet weak var edtInput: UITextField!
    
    var m_dicResultLabel=[UILabel:USNUsnitType]()
    var m_dicNameLabel=[UILabel:USNUsnitType]()
    var m_bShowKeyboard=false
    var edt_delegate:NumInputDelegate?
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.

        SFUsnitLogic.sharedInstance.addObserver(self);
        initUnitName()
        registeTapLable()
        initializeTextFields()
        
        let tap: UITapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(FirstViewController.dismissKeyboard))
        view.addGestureRecognizer(tap)
        self.registerForKeyboardNotifications();
    }
    
    override func viewWillDisappear(animated: Bool){
        NSNotificationCenter.defaultCenter().removeObserver(self,
                                                            name: UIKeyboardDidShowNotification,
                                                            object: nil)
        
        NSNotificationCenter.defaultCenter().removeObserver(self,
                                                            name: UIKeyboardWillHideNotification,
                                                            object: nil)
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func registerForKeyboardNotifications() {
        NSNotificationCenter.defaultCenter().addObserver(
            self,
            selector: #selector(FirstViewController.keyboardWasShown(_:)),
            name: UIKeyboardDidShowNotification,
            object: nil)
        
        NSNotificationCenter.defaultCenter().addObserver(
            self,
            selector: #selector(FirstViewController.keyboardWillBeHidden(_:)),
            name: UIKeyboardWillHideNotification,
            object: nil)
    }
    
    func keyboardWasShown(notification: NSNotification) {
        m_bShowKeyboard = true
    }
    
    func keyboardWillBeHidden(notification: NSNotification) {
        m_bShowKeyboard = false
    }
    
    //Calls this function when the tap is recognized.
    func dismissKeyboard() {
        //Causes the view (or one of its embedded text fields) to resign the first responder status.
        if m_bShowKeyboard{
            //UIApplication.sharedApplication().keyWindow?.endEditing(true)
            edtInput.resignFirstResponder()
        }
    }
    
    // Designate this class as the text fields' delegate
    // and set their keyboards while we're at it.
    func initializeTextFields() {
        
        edt_delegate = NumInputDelegate(edt:edtInput)
        edtInput.delegate = edt_delegate
        edtInput.keyboardType = UIKeyboardType.DecimalPad
    }
    
    func initUnitName(){
        m_dicNameLabel[lblUcm] = 		USNUsnitType.TYPECMETER
        m_dicNameLabel[lblUm] =         USNUsnitType.TYPEMETER
        m_dicNameLabel[lblUkm] =        USNUsnitType.TYPEKMETER
        m_dicNameLabel[lblUft] =        USNUsnitType.TYPEFEET
        m_dicNameLabel[lblUinch] =      USNUsnitType.TYPEINCH
        m_dicNameLabel[lblUmile] =      USNUsnitType.TYPEMILE
        m_dicNameLabel[lblUyard] =      USNUsnitType.TYPEYARD
        m_dicNameLabel[lblUliter] = 	USNUsnitType.TYPELITRE
        m_dicNameLabel[lblUmliter] =    USNUsnitType.TYPEMLITRE
        m_dicNameLabel[lblUgal] =   USNUsnitType.TYPEGAL
        m_dicNameLabel[lblUgram] =  USNUsnitType.TYPEGRAM
        m_dicNameLabel[lblUkgram] = USNUsnitType.TYPEKGRAM
        m_dicNameLabel[lblUlb] =    USNUsnitType.TYPEPOUND
        m_dicNameLabel[lblUoz] =    USNUsnitType.TYPEOZ
        m_dicNameLabel[lblUsqm] =   USNUsnitType.TYPESQM
        m_dicNameLabel[lblUsqcm] =  USNUsnitType.TYPESQCM
        m_dicNameLabel[lblUsqfeet] =     USNUsnitType.TYPESQF
        m_dicNameLabel[lblUcelsius] =    USNUsnitType.TYPECELSI
        m_dicNameLabel[lblUfahrenheit] = USNUsnitType.TYPEFAHRE
        m_dicNameLabel[lblUdollar] = USNUsnitType.TYPEDOLLAR
        m_dicNameLabel[lblUrmb] =    USNUsnitType.TYPERMB
        m_dicNameLabel[lblUsqinch] = USNUsnitType.TYPESQINCH
        m_dicNameLabel[lblExchange] = USNUsnitType.TYPERATE
        
        for (label, type) in m_dicNameLabel{
            label.backgroundColor = UIColor.clearColor()
            label.text = USNUsnitGen.instance()?.getUnitName(type)
        }
    }
    
    func registeTapLable(){
        /*let filteredSubviews = scView.subviews.filter({
            $0.isKindOfClass(UILabel) })
        
        for view in filteredSubviews  {
            let recognizer = UITapGestureRecognizer(target: self, action:#selector(FirstViewController.tapLabel(_:)))
            //recognizer.delegate = self
            view.addGestureRecognizer(recognizer)
        }*/
        
        m_dicResultLabel[lblMeter]     = USNUsnitType.TYPEMETER
        m_dicResultLabel[lblCMeter]    = USNUsnitType.TYPECMETER
        m_dicResultLabel[lblKMeter]    = USNUsnitType.TYPEKMETER
        m_dicResultLabel[lblFeet]      = USNUsnitType.TYPEFEET
        m_dicResultLabel[lblInch]      = USNUsnitType.TYPEINCH
        m_dicResultLabel[lblMile]      = USNUsnitType.TYPEMILE
        m_dicResultLabel[lblYard]      = USNUsnitType.TYPEYARD
        m_dicResultLabel[lblLiter]     = USNUsnitType.TYPELITRE
        m_dicResultLabel[lblMliter]    = USNUsnitType.TYPEMLITRE
        m_dicResultLabel[lblGal]       = USNUsnitType.TYPEGAL
        m_dicResultLabel[lblGram]      = USNUsnitType.TYPEGRAM
        m_dicResultLabel[lblKGram]     = USNUsnitType.TYPEKGRAM
        m_dicResultLabel[lblPound]     = USNUsnitType.TYPEPOUND
        m_dicResultLabel[lblOz]      = USNUsnitType.TYPEOZ
        m_dicResultLabel[lblSqm]       = USNUsnitType.TYPESQM
        m_dicResultLabel[lblSqcm]      = USNUsnitType.TYPESQCM
        m_dicResultLabel[lblSqfeet]    = USNUsnitType.TYPESQF
        m_dicResultLabel[lblCelsius]   = USNUsnitType.TYPECELSI
        m_dicResultLabel[lblFahrenheit] = USNUsnitType.TYPEFAHRE
        m_dicResultLabel[lblDollar]    = USNUsnitType.TYPEDOLLAR
        m_dicResultLabel[lblRmb]       = USNUsnitType.TYPERMB
        m_dicResultLabel[lblSqinch]    = USNUsnitType.TYPESQINCH
        
        for (label, _) in m_dicResultLabel {
            label.backgroundColor = UIColor.clearColor()
            label.layer.borderColor = UIColor.blackColor().CGColor
            label.layer.borderWidth = 1
            //label.text = String("0")
            let recognizer = UITapGestureRecognizer(target: self, action:#selector(FirstViewController.tapLabel(_:)))
            //recognizer.delegate = self
            label.addGestureRecognizer(recognizer)
        }
    }

    @IBAction func tapLabel(sender: UITapGestureRecognizer) {
        let lbl = sender.view as! UILabel;
        
        //g_logouts(LOG_CONSOLE, 3, "tap label:" + String(lbl.accessibilityIdentifier!) + "type: " + String(type))
        UIApplication.sharedApplication().keyWindow?.endEditing(true)
        if let type = m_dicResultLabel[lbl] {
            USNUsnitGen.instance()?.setType(type)
        }

        updatResult()
    }

    @IBAction func onInputChange(sender: UITextField) {
        let input = (sender.text! as NSString).floatValue
        
        USNUsnitGen.instance()?.setInput(input)
        //let meter = USNUsnitGen.instance()?.getResult(USNUsnitType.TYPEMETER)
        //lblMeter.text = meter

        updatResult()
    }
    
    func updatResult()   {
        
        // caculate the max length of label
        
        for (label, type) in m_dicResultLabel{
            label.text = USNUsnitGen.instance()?.getResult(type)
            if label.text == nil {
                print("FirstViewController.updateResult label.text nil type:\(type)")
                continue
            }
            
            if USNUsnitGen.instance()?.getLongType() == type ||
                USNUsnitGen.instance()?.getMassType() == type ||
                USNUsnitGen.instance()?.getSquareType() == type ||
                USNUsnitGen.instance()?.getVolumeType() == type{
                label.textColor = UIColor.blueColor()
            }
            else {
                label.textColor = UIColor.blackColor()
            }
            
            //let cut:Bool = false
            /*
            if cut {
                let str:NSString = "9"
                let fontAttributes = [NSFontAttributeName: lblMeter.font]
                let lblwidth = lblMeter.bounds.size.width
                let wwidth = str.sizeWithAttributes(fontAttributes).width
                let chars:Int = Int( lblwidth/wwidth )
                if label.text!.characters.count < chars {
                    continue
                }
                label.text = label.text!.substringToIndex(label.text!.startIndex.advancedBy(chars))
            }
             */
        }

    }
    
    func HandleResult(type: USNUsnitEventType, _ result:String) -> Bool{
        switch type {
        case USNUsnitEventType.CBDOLLARRT:
            lblDollar.text = result
            break
        case USNUsnitEventType.CBRMBRT:
            lblRmb.text = result
            break
        case USNUsnitEventType.CBRATEINFO:
            lblExchange.text = result
        default:
            break
        }
        return true;
    }
    
 }

