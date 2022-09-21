import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JFrame;

/*
 * The Mandelbrot fractal can be zoomed in or zoomed out by 
 * adjusting the min and max values. The initial value of the 
 * accuracy variable should be 1000. The accuracy of the 
 * Mandelbrot fractal in this case would be the number of 
 * iterations to check if the complex number approaches infinity.
 * The shape of the Mandelbrot curve begins to form around 
 * accuracy = 10-20.
 * 
 * The iter variable changes the number of iterations the 
 * mandelbrotRecursion goes through. The iter variable, in a 
 * sense, changes the quality of the Mandelbrot. It changes
 * the number of pixels the graph is split into. 
 * 
 * Normally, I like to keep the # of iterations at 11 
 * because it's as much as my computer can handle. 
 * 
 * Here are a few interesting configurations:
 * 
 * DEFAULT CONFIG:
 *  int length = 1000;
	int height = 1000;
	int accuracy = 1000;
	int iter = 11;
	double minX = -2;
	double maxX = 2;
	double minY = -2;
	double maxY = 2;
 * 
 * MINI-MANDELBROT ZOOM:
 *  int length = 1000;
	int height = 1000;
	int accuracy = 1000;
	int iter = 11;
	double minX = -1.833;
	double maxX = -1.667;
	double minY = -0.083;
	double maxY = 0.083;
 * 
 * 500x500 MANDELBROT ZOOM:
 *  int length = 500;
	int height = 500;
	int accuracy = 1000;
	int iter = 11;
	double minX = -2;
	double maxX = 2;
	double minY = -2;
	double maxY = 2;
 * 
 * ZOOM (x0.025) ON TIP OF MANDELBROT:
 *  int length = 500;
	int height = 500;
	int accuracy = 1000;
	int iter = 11;
	double minX = -1.425;
	double maxX = -1.375;
	double minY = -0.025;
	double maxY = 0.025;
	
 * ^Increase the accuracy on the zoom above to see 
 * how the accuracy variable works 
 * (change the accuracy to 5000 as an example--
 * it takes some time to load)
 * 
 * ZOOM (x0.00625) ON TIP OF MANDELBROT:
 *  int length = 500;
	int height = 500;
	int accuracy = 1000;
	int iter = 11;
	double minX = -1.40625;
	double maxX = -1.39375;
	double minY = -0.00625;
	double maxY = 0.006255;
 * 
 * ^Increase the accuracy on the zoom above to see 
 * how the accuracy variable works 
 * (change the accuracy to 5000 as an example--
 * it takes some time to load)
 * 
 * ZOOM (xZOOMFACTOR) ON TIP OF MANDELBROT:
 *  double zoomFactor = 0.5;
	double xCenter = -1.401155;
	double yCenter = 0;
	int length = 500;
	int height = 500;
	int accuracy = 1000;
	int iter = 11;
	double minX = xCenter - 1/zoomFactor;
	double maxX = xCenter + 1/zoomFactor;
	double minY = yCenter - 1/zoomFactor;
	double maxY = yCenter + 1/zoomFactor;
 * 
 * ^The constant -1.401155 is one of the Feigenbaum points. The set is self-similar at that point
 * (converging to an limit of itself). 
 * 
 * ^Try changing zoomFactor to 10,100,1000,10000... Change the accuracy to refine the resolution of the
 * brot.
 * 
 * ZOOM OF MANDELBROT AT XCENTER AND YCENTER:
 *  double zoomFactor = 0.5;
	double xCenter = -0.75;
	double yCenter = 0;
	int length = 500;
	int height = 500;
	int accuracy = 1000;
	int iter = 11;
	double minX = xCenter - 1/zoomFactor;
	double maxX = xCenter + 1/zoomFactor;
	double minY = yCenter - 1/zoomFactor;
	double maxY = yCenter + 1/zoomFactor; 
 * 
 * These coordinates are from http://paulbourke.net/fractals/mandelbrot/.
 * Check them out...
 * 
 * Keep the accuracy of the following to 5000-10,000 for the best results.
 * 
 * (-0.170337,-1.06506) -- tendrils (lightning)
 * (0.42884,-0.231345) -- ZOOM 2
 * (-0.761574,-0.0847596) -- Zoom 4 - spirals
 * 
 * 
 * 
 */


public class Mandelbrot extends JFrame{
  // -- BEGIN CONFIG BLOCK --
	double zoomFactor = 500;
	double xCenter = -0.761574;
	double yCenter = -0.0847596;
	int length = 500;
	int height = 500;
	int accuracy = 1000;
	int iter = 14;
	double minX = xCenter - 1/zoomFactor;
	double maxX = xCenter + 1/zoomFactor;
	double minY = yCenter - 1/zoomFactor;
	double maxY = yCenter + 1/zoomFactor;
	// -- END OF CONFIG BLOCK --
	
	Graphics g;
	static int infinityTest = 5;
	int halfLength = length/2;
	int halfHeight = height/2;
	
	int stepLength = (int)(length/(maxX - minX));
	int stepHeight = (int)(height/(maxY - minY));
	
	public Mandelbrot() {
		super("The Mandelbrot Fractal");
		setSize(length,height);
		setVisible(true);
	}
	
	public void paint(Graphics gi) {
		g = gi;
		super.paint(g);
		g.setColor(Color.RED);
		g.drawLine((int)(halfLength + (stepLength)*(0 - (minX+maxX)/2)),
				(int)(halfHeight - stepHeight*(5 - (minY+maxY)/2)),
				(int)(halfLength + (stepLength)*(0 - (minX+maxX)/2)),
				(int)(halfHeight - stepHeight*(-5 - (minY+maxY)/2)));
		g.drawLine((int)(halfLength + (stepLength)*(-5 - (minX+maxX)/2)),
				(int)(halfHeight - stepHeight*(0 - (minY+maxY)/2)),
				(int)(halfLength + (stepLength)*(5 - (minX+maxX)/2)),
				(int)(halfHeight - stepHeight*(0 - (minY+maxY)/2)));
		mandelbrotRecursion(iter,minX,minY,maxX,maxY);
	}
	
	public void mandelbrotRecursion(int iterations, double x1, double y1, double x2, double y2) {
		if(iterations == 1) {
			if(isInBrotSet((x1 + x2)/2,(y1+y2)/2,accuracy)) {
				fillPoint(x1,y1,x2-x1,y2-y1);
			}
		}else {
			mandelbrotRecursion(iterations - 1, x1, y1, (x1 + x2)/2, (y1 + y2)/2);
			mandelbrotRecursion(iterations - 1, (x1 + x2)/2, y1, x2, (y1 + y2)/2);
			mandelbrotRecursion(iterations - 1, x1, (y1 + y2)/2, (x1 + x2)/2, y2);
			mandelbrotRecursion(iterations - 1, (x1 + x2)/2, (y1 + y2)/2, x2, y2);
		}
	}
	
	public void fillPoint(double x,double y, double sizex, double sizey) {
		g.setColor(Color.BLACK);
		g.fillRect((int)(halfLength + (stepLength)*(x - (minX+maxX)/2)),(int)(halfHeight - stepHeight*(y - (minY+maxY)/2)),(int)(sizex*stepLength)+1,(int)(sizey*stepHeight)+1);
	}
	
	public static boolean isInBrotSet(double real, double imaginary, int iterations) {
		double zn = 0;
		double zni = 0;
		double zn1 = 0, zn1i = 0;
		for(int i = 0; i < iterations; i++) {
			zn = zn1;
			zni = zn1i;
			
			zn1 = Math.pow(zn, 2) - Math.pow(zni, 2) + real;
			zn1i = 2*zn*zni + imaginary;
			
			if(Math.log(Math.abs(zn1)) > infinityTest && Math.log(Math.abs(zn1i)) > infinityTest) {
				return false;
			}
		}
		
		if(Math.log(Math.abs(zn1)) < infinityTest && Math.log(Math.abs(zn1i)) < infinityTest) {
			return true;
		}else {
			return false;
		}
	}
	
	public static void main(String[] args) {
		Mandelbrot application = new Mandelbrot();
		application.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}
