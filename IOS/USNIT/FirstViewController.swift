//
//  FirstViewController.swift
//  USNIT
//
//  Created by dyno on 3/26/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation
import UIKit


class FirstViewController: UIViewController {

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
    
    @IBOutlet weak var edtInput: UITextField!
    
    var m_dicLabel=[UILabel:Int32]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        let filteredSubviews = self.view.subviews.filter({
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
        m_dicLabel[lblSqinch]    = TYPE_SQINCH
        
        initializeTextFields()
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
    }
    
 }

