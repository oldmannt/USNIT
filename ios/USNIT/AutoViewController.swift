//
//  AutoViewController.swift
//  usnit_app
//
//  Created by dyno on 5/28/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation
import UIKit

class AutoViewController: UIViewController {
    
    @IBOutlet weak var viewInput: UIView!
    @IBOutlet weak var viewUnit: UIView!
    var genViewInput:GBViewImp?
    var genViewUnit:GBViewImp?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        //UITextBorderStyle
        /*
        let myTextField = UITextField(/*frame: CGRect(x: 20, y: 20, width: 100.0, height: 40.00)*/)
        //myTextField.frame = CGRect(x: 20, y: 0, width: 100.0, height: 40.00)
        //myTextField.backgroundColor = UIColor.grayColor()
        myTextField.placeholder="Enter"
        //myTextField.text = "    Enter here"
        myTextField.borderStyle = UITextBorderStyle.RoundedRect
        //myTextField.secureTextEntry=true
        viewInput.addSubview(myTextField)
        myTextField.text = "input"
        myTextField.translatesAutoresizingMaskIntoConstraints = false;
        let cs = myTextField.constraints
        
        let conCenter = NSLayoutConstraint(item: myTextField, attribute: .CenterX, relatedBy: .Equal, toItem:viewInput , attribute: .CenterX, multiplier: 1, constant: 0)
        conCenter.identifier = "centerText"
        //conCenter.priority = 250
        let conWidth = NSLayoutConstraint(item: myTextField, attribute: .Width, relatedBy: .Equal, toItem:viewInput , attribute: .Width, multiplier: 1, constant: 0)
        conWidth.identifier = "centerWidth"
        //conWidth.priority = 250
        let conTop = NSLayoutConstraint(item: myTextField, attribute: .Top, relatedBy: .Equal, toItem:viewInput , attribute: .Top, multiplier: 1, constant: 40)
        conTop.identifier = "centerTop"
        //conTop.priority = 250
        let conHeight = NSLayoutConstraint(item: myTextField, attribute: .Height, relatedBy: .Equal, toItem:nil , attribute: .Height, multiplier: 1, constant: 30)
        conHeight.identifier = "centerHeight"
        //conHeight.priority = 250
        
        viewInput.addConstraint(conCenter)
        viewInput.addConstraint(conWidth)
        viewInput.addConstraint(conTop)
        viewInput.addConstraint(conHeight)
        
        //viewInput.addConstraint(NSLayoutConstraint(item: myTextField, attribute: .CenterX, relatedBy: .Equal, toItem: viewInput, attribute: .CenterX, multiplier: 1, constant: 0))
        */
        
        let res_path = NSBundle.mainBundle().pathForResource("ui", ofType: "json")
        if (res_path == nil){
            GBLogGen.instance()?.logerrf("ui.json not found \(#file) \(#function) \(#line)");
            return;
        }
        GBLogGen.instance()?.logerrf("\(res_path)");
        //GBUiManagerGen.instance()?.initialize(res_path!, factory: nil)
        genViewInput = GBViewImp(id:"input_view", view: viewInput, constroller: self)
        genViewUnit = GBViewImp(id:"unit_view", view: viewUnit, constroller: self)

        GBUiManagerGen.instance()?.inject("input_view", view: genViewInput)
        GBUiManagerGen.instance()?.inject("unit_view", view: genViewUnit)
        
        genViewInput!.addSubView("input_text");
        genViewInput!.addSubView("time_label");
        //USNUsnitGen.instance()?.buildView("unit_view")
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

}
