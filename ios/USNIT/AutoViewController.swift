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

        SFUsnitLogic.sharedInstance
        
        let res_path = NSBundle.mainBundle().pathForResource("ui", ofType: "json")
        if (res_path == nil){
            GBLogGen.instance()?.logerrf("ui.json not found \(#file) \(#function) \(#line)");
            return;
        }

        GBUiManagerGen.instance()?.initialize(res_path!, factory: GBViewFactoryImp.instance)
        genViewInput = GBViewImp(id:"input_view", view: viewInput, constroller: self)
        genViewUnit = GBViewImp(id:"unit_view", view: viewUnit, constroller: self)

        GBUiManagerGen.instance()?.inject(genViewInput)
        GBUiManagerGen.instance()?.inject(genViewUnit)
        
        USNUsnitGen.instance()?.buildView("input_view")
        USNUsnitGen.instance()?.buildView("unit_view")
        
        m_ad = AdmobBanner(root: self)
        m_ad.didLoad("ca-app-pub-4953725946697554/8856468622")
        
        // set height of scview
        let offsety = (self.tabBarController?.tabBar.frame.size.height)! + m_ad.bannerView.frame.size.height
        view.addConstraint(NSLayoutConstraint(item: viewSC, attribute: .Bottom,
            relatedBy: .Equal, toItem: view , attribute: .Bottom, multiplier: 1, constant: -offsety))
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        
        var maxHeight : CGFloat = 0
        for view in viewUnit.subviews {
            let newHeight = view.frame.origin.y + view.frame.height
            if newHeight > maxHeight {
                maxHeight = newHeight
            }
        }
        // set content size
        
        viewSC.contentSize = CGSize(width: viewSC.contentSize.width, height: maxHeight)
    }

}
