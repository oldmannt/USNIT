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
    
    var m_dicResultLabel=[UILabel:Int32]()
    var m_dicNameLabel=[UILabel:Int32]()
    var m_bShowKeyboard=false
    var edt_delegate:NumInputDelegate?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.

        initUnitName()
        registeTapLable()
        initializeTextFields()
        
        SFUsnitLogic.sharedInstance.addObserver(self);
        
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
        edtInput.keyboardType = UIKeyboardType.NumberPad
    }
    
    func initUnitName(){
        m_dicNameLabel[lblUcm] = 		TYPE_CMETER
        m_dicNameLabel[lblUm] =         TYPE_METER
        m_dicNameLabel[lblUkm] =        TYPE_KMETER
        m_dicNameLabel[lblUft] =        TYPE_FEET
        m_dicNameLabel[lblUinch] =      TYPE_INCH
        m_dicNameLabel[lblUmile] =      TYPE_MILE
        m_dicNameLabel[lblUyard] =      TYPE_YARD
        m_dicNameLabel[lblUliter] = 	TYPE_LITRE
        m_dicNameLabel[lblUmliter] =    TYPE_MLITRE
        m_dicNameLabel[lblUgal] =   TYPE_GAL
        m_dicNameLabel[lblUgram] =  TYPE_GRAM
        m_dicNameLabel[lblUkgram] = TYPE_KGRAM
        m_dicNameLabel[lblUlb] =    TYPE_POUND
        m_dicNameLabel[lblUoz] =    TYPE_OZ
        m_dicNameLabel[lblUsqm] =   TYPE_SQM
        m_dicNameLabel[lblUsqcm] =  TYPE_SQCM
        m_dicNameLabel[lblUsqfeet] =     TYPE_SQF
        m_dicNameLabel[lblUcelsius] =    TYPE_CELSI
        m_dicNameLabel[lblUfahrenheit] = TYPE_FAHRE
        m_dicNameLabel[lblUdollar] = TYPE_DOLLAR
        m_dicNameLabel[lblUrmb] =    TYPE_RMB
        m_dicNameLabel[lblUsqinch] = TYPE_SQINCH
        m_dicNameLabel[lblExchange] = TYPE_RATE
        
        for (label, type) in m_dicNameLabel{
            label.backgroundColor = UIColor.clearColor()
            label.text = SFUsnitLogic.sharedInstance.getUsnitName(type)
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
        
        m_dicResultLabel[lblMeter]     = TYPE_METER
        m_dicResultLabel[lblCMeter]    = TYPE_CMETER
        m_dicResultLabel[lblKMeter]    = TYPE_KMETER
        m_dicResultLabel[lblFeet]      = TYPE_FEET
        m_dicResultLabel[lblInch]      = TYPE_INCH
        m_dicResultLabel[lblMile]      = TYPE_MILE
        m_dicResultLabel[lblYard]      = TYPE_YARD
        m_dicResultLabel[lblLiter]     = TYPE_LITRE
        m_dicResultLabel[lblMliter]    = TYPE_MLITRE
        m_dicResultLabel[lblGal]       = TYPE_GAL
        m_dicResultLabel[lblGram]      = TYPE_GRAM
        m_dicResultLabel[lblKGram]     = TYPE_KGRAM
        m_dicResultLabel[lblPound]     = TYPE_POUND
        m_dicResultLabel[lblOz]      = TYPE_OZ
        m_dicResultLabel[lblSqm]       = TYPE_SQM
        m_dicResultLabel[lblSqcm]      = TYPE_SQCM
        m_dicResultLabel[lblSqfeet]    = TYPE_SQF
        m_dicResultLabel[lblCelsius]   = TYPE_CELSI
        m_dicResultLabel[lblFahrenheit] = TYPE_FAHRE
        m_dicResultLabel[lblDollar]    = TYPE_DOLLAR
        m_dicResultLabel[lblRmb]       = TYPE_RMB
        m_dicResultLabel[lblSqinch]    = TYPE_SQINCH
        
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
        
        if let type = m_dicResultLabel[lbl] {
            UsnitSetType(type);
        }

        updatResult()
    }

    @IBAction func onInputChange(sender: UITextField) {
        let input = (sender.text! as NSString).floatValue
        
        SFUsnitLogic.sharedInstance.setUsnitInput(input)
        //let meter = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_METER)
        //lblMeter.text = meter

        updatResult()
    }
    
    func updatResult()   {
        
        // caculate the max length of label
        
        let cut:Bool = false
        for (label, type) in m_dicResultLabel{
            label.text = SFUsnitLogic.sharedInstance.getUsnitResult(type)
            if label.text == nil {
                print("FirstViewController.updateResult label.text nil type:\(type)")
                continue
            }
            
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
        }
        /*
        lblMeter.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_METER)
        lblCMeter.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_CMETER)
        lblKMeter.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_KMETER)
        lblFeet.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_FEET)
        lblInch.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_INCH)
        lblMile.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_MILE)
        lblYard.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_YARD)
        lblSqm.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_SQM)
        lblSqcm.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_SQCM)
        lblSqfeet.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_SQF)
        lblLiter.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_LITRE)
        lblMliter.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_MLITRE)
        lblGal.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_GAL)
        lblGram.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_GRAM)
        lblKGram.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_KGRAM)
        lblPound.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_POUND)
        lblOz.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_OZ)
        lblCelsius.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_CELSI)
        lblFahrenheit.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_FAHRE)
        lblSqinch.text = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_SQINCH)
        
        if let dollor = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_DOLLAR){
            lblDollar.text = dollor
        }
        if  let rmb = SFUsnitLogic.sharedInstance.getUsnitResult(TYPE_RMB){
            lblRmb.text = rmb
        }
 */
    }
    
    func HandleResult(type: Int32, _ result:String) -> Bool{
        switch type {
        case CB_DOLLAR_RT:
            lblDollar.text = result
            break
        case CB_RMB_RT:
            lblRmb.text = result
            break
        case CB_RATEINFO:
            lblExchange.text = result
        default:
            break
        }
        return true;
    }
    
 }

