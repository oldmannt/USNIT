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
        
        UsnitSetInput(input)
        lblMeter.text = SFUsnitLogic.sharedInstance.GetUsnitResult(TYPE_METER);
        
    }
    
 }

