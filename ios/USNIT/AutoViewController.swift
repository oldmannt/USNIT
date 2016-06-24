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
    
    @IBOutlet weak var viewSC: UIScrollView!
    @IBOutlet weak var viewInput: UIView!
    @IBOutlet weak var viewUnit: UIView!
    var genViewInput:GBViewImp?
    var genViewUnit:GBViewImp?
    var m_ad:AdmobBanner!
    
    override func viewDidLoad() {
        super.viewDidLoad()
               
        let res_path = NSBundle.mainBundle().pathForResource("ui", ofType: "json")
        if (res_path == nil){
            GBLogGen.instance()?.logerrf("ui.json not found \(#file) \(#function) \(#line)");
            return;
        }
        
        GBPlatformUtilityGen.instance()?.setPlatofrmExcutor(IOSExecutor())

        GBUiManagerGen.instance()?.initialize(res_path!, factory: GBViewFactoryImp.instance)
        genViewInput = GBViewImp(id:"input_view", view: viewInput, constroller: self)
        genViewUnit = GBViewImp(id:"unit_view", view: viewUnit, constroller: self)

        GBUiManagerGen.instance()?.inject(genViewInput)
        GBUiManagerGen.instance()?.inject(genViewUnit)
        
        SFUsnitLogic.sharedInstance
        
        //*
        m_ad = AdmobBanner(root: self)
        m_ad.didLoad("ca-app-pub-4953725946697554/8856468622")
        
        // set height of scview
        let offsety = (self.tabBarController?.tabBar.frame.size.height)! + m_ad.bannerView.frame.size.height
        view.addConstraint(NSLayoutConstraint(item: viewSC, attribute: .Bottom,
            relatedBy: .Equal, toItem: view , attribute: .Bottom, multiplier: 1, constant: -offsety))
        //*/
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        
        var maxHeight : CGFloat = viewSC.contentSize.height
        if maxHeight < viewUnit.frame.height{
            maxHeight = viewUnit.frame.height
        }
        
        for subview in viewUnit.subviews {
            let newHeight = subview.frame.origin.y + subview.frame.height
            if newHeight > maxHeight {
                maxHeight = newHeight
            }
        }
        //GBLogGen.instance()?.logerrf("old content height: \(viewSC.contentSize.height) frame height: \(viewUnit.frame.height)");
        // set content size
        viewSC.contentSize = CGSize(width: viewSC.contentSize.width, height: maxHeight)
        viewUnit.frame = CGRectMake(viewUnit.frame.origin.x,viewUnit.frame.origin.y,
                                    viewUnit.frame.width, maxHeight)
        //GBLogGen.instance()?.logerrf("content height: \(maxHeight)");
        //GBLogGen.instance()?.logerrf("view rame:\(viewUnit.frame.origin.x) \(viewUnit.frame.origin.y) \(viewUnit.frame.width) \(maxHeight)");
    }

}
