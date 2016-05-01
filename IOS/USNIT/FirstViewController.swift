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
    
    var m_dicLabel=[UILabel:Int32]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        let filteredSubviews = scView.subviews.filter({
            $0.isKindOfClass(UILabel) })

        for view in filteredSubviews  {
            let recognizer = UITapGestureRecognizer(target: self, action:#selector(FirstViewController.tapLabel(_:)))
            //recognizer.delegate = self
            view.addGestureRecognizer(recognizer)
        }
        
        m_dicLabel[lblMeter]     = TYPE_METER
        m_dicLabel[lblCMeter]    = TYPE_CMETER
        m_dicLabel[lblKMeter]    = TYPE_KMETER
        m_dicLabel[lblFeet]      = TYPE_FEET
        m_dicLabel[lblInch]      = TYPE_INCH
        m_dicLabel[lblMile]      = TYPE_MILE
        m_dicLabel[lblYard]      = TYPE_YARD
        m_dicLabel[lblLiter]     = TYPE_LITRE
        m_dicLabel[lblMliter]    = TYPE_MLITRE
        m_dicLabel[lblGal]       = TYPE_GAL
        m_dicLabel[lblGram]    = TYPE_GRAM
        m_dicLabel[lblKGram]   = TYPE_KGRAM
        m_dicLabel[lblPound]   = TYPE_POUND
        m_dicLabel[lblOz]      = TYPE_OZ
        m_dicLabel[lblSqm]       = TYPE_SQM
        m_dicLabel[lblSqcm]      = TYPE_SQCM
        m_dicLabel[lblSqfeet]    = TYPE_SQF   
        m_dicLabel[lblCelsius]   = TYPE_CELSI 
        m_dicLabel[lblFahrenheit] = TYPE_FAHRE
        m_dicLabel[lblDollar]    = TYPE_DOLLAR
        m_dicLabel[lblRmb]       = TYPE_RMB
        m_dicLabel[lblSqinch]    = TYPE_SQINCH
        
        initializeTextFields()
        
        SFUsnitLogic.sharedInstance.AddObserver(self);
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    // Designate this class as the text fields' delegate
    // and set their keyboards while we're at it.
    func initializeTextFields() {
        
        let edt_delegate = NumInputDelegate(edt: edtInput)
        edtInput.delegate = edt_delegate
        edtInput.keyboardType = UIKeyboardType.NumberPad
    }
    
    // Dismiss the keyboard when the user taps the "Return" key or its equivalent
    // while editing a text field.
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true;
    }

    @IBAction func tapLabel(sender: UITapGestureRecognizer) {
        let lbl = sender.view as! UILabel;
        let type = m_dicLabel[lbl]!
        
        //g_logouts(LOG_CONSOLE, 3, "tap label:" + String(lbl.accessibilityIdentifier!) + "type: " + String(type))
        
        UsnitSetType(type);

        updatResult()
    }

    @IBAction func onInputChange(sender: UITextField) {
        let input = (sender.text! as NSString).floatValue
        
        SFUsnitLogic.sharedInstance.SetUsnitInput(input)
        //let meter = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_METER)
        //lblMeter.text = meter

        updatResult()
    }
    
    func updatResult()   {
        
        lblMeter.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_METER)
        lblCMeter.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_CMETER)
        lblKMeter.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_KMETER)
        lblFeet.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_FEET)
        lblInch.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_INCH)
        lblMile.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_MILE)
        lblYard.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_YARD)
        lblSqm.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_SQM)
        lblSqcm.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_SQCM)
        lblSqfeet.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_SQF)
        lblLiter.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_LITRE)
        lblMliter.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_MLITRE)
        lblGal.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_GAL)
        lblGram.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_GRAM)
        lblKGram.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_KGRAM)
        lblPound.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_POUND)
        lblOz.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_OZ)
        lblCelsius.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_CELSI)
        lblFahrenheit.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_FAHRE)
        lblSqinch.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_SQINCH)
        
        if let dollor = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_DOLLAR){
            lblDollar.text = dollor
        }
        if  let rmb = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_RMB){
            lblRmb.text = rmb
        }
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

