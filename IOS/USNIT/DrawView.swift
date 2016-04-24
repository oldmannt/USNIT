//
//  DrawView.swift
//  USNIT
//
//  Created by dyno on 4/23/16.
//  Copyright © 2016 dyno. All rights reserved.
//

import UIKit

class DrawView: UIView {
    func drawLine(xb:CGFloat, xe:CGFloat, yb:CGFloat, ye:CGFloat, inContext ctx: CGContext) {
        CGContextBeginPath(ctx)
        CGContextMoveToPoint(ctx, xb, yb)
        CGContextAddLineToPoint(ctx, xe, ye)
        CGContextStrokePath(ctx)
    }
    
    override func drawRect(rect: CGRect){
        super.drawRect(rect)
        
        let context = UIGraphicsGetCurrentContext()
        let scale = self.window!.screen.scale // assumed to be in a UIView drawRect
        let width = 1 / scale
        let centerChoice: CGFloat = scale % 2 == 0 ? 4 : 2
        let offset = scale / centerChoice * width
        
        CGContextSetLineWidth(context, width)
        CGContextSetStrokeColorWithColor(context, UIColor.blackColor().CGColor)
        
        let rcEdt:CGRect = edtInput.convertRect(edtInput.bounds, toView: nil)
        
        //let insetEdt = edtInput.alignmentRectInsets()
        let rcYard = lblYard.convertRect(lblYard.bounds, toView:nil)
        let rcSqm = lblSqm.convertRect(lblSqm.bounds, toView: nil)
        let rcSqinch = lblSqinch.convertRect(lblSqinch.bounds, toView: nil)
        let rcLiter = lblLiter.convertRect(lblLiter.bounds, toView: nil)
        let rcMliter = lblMliter.convertRect(lblMliter.bounds, toView: nil)
        let rcGram = lblGram.convertRect(lblGram.bounds, toView: nil)
        let rcOz = lblOz.convertRect(lblOz.bounds, toView: nil)
        let rcCelsius = lblCelsius.convertRect(lblCelsius.bounds, toView: nil)
        
        let xb: CGFloat = rcEdt.minX + offset
        let xe: CGFloat = rcEdt.maxX + offset
        
        let y1: CGFloat = rcYard.maxY + (-rcYard.maxY+rcSqm.minY-1)/2 + offset
        let y2: CGFloat = rcSqinch.maxY + (-rcSqinch.maxY+rcLiter.minY-1)/2 + offset
        let y3: CGFloat = rcMliter.maxY + (-rcMliter.maxY+rcGram.minY-1)/2 + offset
        let y4: CGFloat = rcOz.maxY + (-rcOz.maxY+rcCelsius.minY-1)/2 + offset
        //let y3: CGFloat = (.maxY-.minY-1)/2 + offset
        
        drawLine(xb, xe: xe, yb: y1, ye: y1, inContext: context!)
        drawLine(xb, xe: xe, yb: y2, ye: y2, inContext: context!)
        drawLine(xb, xe: xe, yb: y3, ye: y3, inContext: context!)
        drawLine(xb, xe: xe, yb: y4, ye: y4, inContext: context!)
        
    }
    @IBOutlet weak var lblSqm: UILabel!
    @IBOutlet weak var lblSqinch: UILabel!
    @IBOutlet weak var lblLiter: UILabel!
    @IBOutlet weak var lblMliter: UILabel!
    @IBOutlet weak var lblGram: UILabel!
    @IBOutlet weak var lblOz: UILabel!
    @IBOutlet weak var lblCelsius: UILabel!
    @IBOutlet weak var lblYard: UILabel!
    @IBOutlet weak var edtInput: UITextField!
}
