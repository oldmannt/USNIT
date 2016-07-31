//
//  scrollsubview.swift
//  usnit_app
//
//  Created by dyno on 7/29/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import Foundation
import UIKit

class ScrollViewTap: UIScrollView {
    internal override func touchesShouldCancelInContentView(view: UIView) -> Bool{
        return false;
    }
    
    internal override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?){
        nextResponder()?.touchesBegan(touches, withEvent: event)
        super.touchesBegan(touches, withEvent: event)
    }
    internal override func touchesMoved(touches: Set<UITouch>, withEvent event: UIEvent?){
        nextResponder()?.touchesMoved(touches, withEvent: event)
        super.touchesMoved(touches, withEvent: event)
    }
    internal override func touchesEnded(touches: Set<UITouch>, withEvent event: UIEvent?){
        nextResponder()?.touchesEnded(touches, withEvent: event)
        super.touchesEnded(touches, withEvent: event)
    }
}