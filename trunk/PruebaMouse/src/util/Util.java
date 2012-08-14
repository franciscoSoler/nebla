package util;

import javax.swing.JComponent;

public class Util {

	public Util () {
		
	}
	
	public boolean seSuperponen (JComponent componente1, JComponent componente2) {
		int x1 = componente1.getX();
		int y1 = componente1.getY();
		int width1 = componente1.getWidth();
		int height1 = componente1.getHeight();

		int x2 = componente2.getX();
		int y2 = componente2.getY();
		int width2 = componente2.getWidth();
		int height2 = componente2.getHeight();
	
		if ((x2+width2 > x1) && (x1+width1 > x2) && (y2+height2 > y1) && (y1+height1 > y2)) {
			return true;
		}
		return false;
	}
}
