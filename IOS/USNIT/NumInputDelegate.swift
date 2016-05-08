//
//  NumInputDelegate.swift
//  USNIT
//
//  Created by dyno on 4/9/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation
import UIKit

class NumInputDelegate: NSObject, UITextFieldDelegate {
    
    private var edtInput : UITextField?
    
    init(edt: UITextField){
        edtInput = edt;
    }
    
    // Dismiss the keyboard when the user taps the "Return" key or its equivalent
    // while editing a text field.
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true;
    }
    
    func textField(textField: UITextField,
                   shouldChangeCharactersInRange range: NSRange,
                                                 replacementString string: String)
        -> Bool
    {
        // We ignore any change that doesn't add characters to the text field.
        // These changes are things like character deletions and cuts, as well
        // as moving the insertion point.
        //
        // We still return true to allow the change to take place.
        if string.characters.count == 0 {
            return true
        }
        
        // Check to see if the text field's contents still fit the constraints
        // with the new content added to it.
        // If the contents still fit the constraints, allow the change
        // by returning true; otherwise disallow the change by returning false.
        let currentText = textField.text ?? ""
        let prospectiveText = (currentText as NSString).stringByReplacingCharactersInRange(range, withString: string)
        
        switch textField {
            
        // allow only positive number
        case edtInput!:
            if !prospectiveText.containsOnlyCharactersIn(".0123456789"){
                return false
            }
            
            if prospectiveText.hasPrefix(".0"){
                return false
            }
            
            // allow only one "."
            if string == "." && currentText.containsString("."){
                return false
            }
            
            return true;
            
        default:
            return true
        }
        
        
        
    }
}