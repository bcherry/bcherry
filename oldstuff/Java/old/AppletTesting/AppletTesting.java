/* <applet code="AppletTesting.class" width=150 height=120></applet> */
import java.awt.*;
import javax.swing.*;

public class AppletTesting extends JApplet {
    public void paint (Graphics g) {
        drawChair(10,10,50,100,g);
        drawChair(70,10,25,50,g);
        drawChair(105,10,10,20,g);
    }
    void drawChair (int l, int t, int w, int h, Graphics g) {
        int thickness = w/10 + 1;
        g.fillRect(l,t+h/2,w,thickness);
        g.fillRect(l+w-thickness,t,thickness,h);
        g.fillRect(l,t+h/2,thickness,h/2);
    }
}