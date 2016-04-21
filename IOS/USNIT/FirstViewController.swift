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
    
    @IBOutlet weak var edtInput: UITextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        initializeTextFields();
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
    
    // Tap outside a text field to dismiss the keyboard
    // ------------------------------------------------
    // By changing the underlying class of the view from UIView to UIControl,
    // the view can respond to events, including Touch Down, which is
    // wired to this method.
    @IBAction func userTappedBackground(sender: AnyObject) {
        view.endEditing(true)
    }
    
    // Dismiss the keyboard when the user taps the "Return" key or its equivalent
    // while editing a text field.
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true;
    }

    @IBAction func onInputChange(sender: UITextField) {
        let input = (sender.text! as NSString).floatValue
        
        SFUsnitLogic.sharedInstance.SetUsnitInput(input)
        //let meter = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_METER)
        //lblMeter.text = meter

        var dicTypeResult=[Int32:String]()
        
        for i in TYPE_METER..<TYPE_MAX {
            dicTypeResult[i] = SFUsnitLogic.sharedInstance.GetUsnitResult(i)
        }
        
        lblMeter.text = dicTypeResult[TYPE_METER]
        lblCMeter.text = dicTypeResult[TYPE_CMETER]
        lblKMeter.text = dicTypeResult[TYPE_KGRAM]
        lblFeet.text = dicTypeResult[TYPE_FEET]
        lblInch.text = dicTypeResult[TYPE_INCH]
        lblMile.text = dicTypeResult[TYPE_MILE]
        lblYard.text = dicTypeResult[TYPE_YARD]
        lblSqm.text = dicTypeResult[TYPE_SQM]
        lblSqcm.text = dicTypeResult[TYPE_SQCM]
        lblSqfeet.text = dicTypeResult[TYPE_SQF]
        lblLiter.text = dicTypeResult[TYPE_LITRE]
        lblMliter.text = dicTypeResult[TYPE_MLITRE]
        lblGal.text = dicTypeResult[TYPE_GAL]
        lblGram.text = dicTypeResult[TYPE_GRAM]
        lblKGram.text = dicTypeResult[TYPE_KGRAM]
        lblPound.text = dicTypeResult[TYPE_POUND]
        lblOz.text = dicTypeResult[TYPE_OZ]
        lblCelsius.text = dicTypeResult[TYPE_CELSI]
        lblFahrenheit.text = dicTypeResult[TYPE_FAHRE]
    }
    
 }

