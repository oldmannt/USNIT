//
//  UsnitTabBar.swift
//  USNIT
//
//  Created by dyno on 5/6/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation
import UIKit

class UsnitTabBarCtl: UITabBarController{
    override func viewWillLayoutSubviews() {
        var tabFrame: CGRect = self.tabBar.frame
        tabFrame.size.height = 21
        tabFrame.origin.y = self.view.frame.size.height - 21
        self.tabBar.frame = tabFrame
    }
}