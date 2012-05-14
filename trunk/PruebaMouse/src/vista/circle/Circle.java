package vista.circle;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Ellipse2D;

import javax.swing.JPanel;

public class Circle extends JPanel {

	private static final long serialVersionUID = 1L;
	
	private Point center;
	private int radio;
	
	public Circle (int radio, int x, int y) {
		this.center = new Point(x,y);
		this.radio = radio;
		System.out.println("Creando un circulo");
	}
	
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		System.out.println("Dibujando un circle");
		Graphics2D comp2D = (Graphics2D)g;
		
		comp2D.setColor(Color.red);
		Ellipse2D.Float sign1 = new Ellipse2D.Float(center.x, center.y, radio, radio);
		comp2D.fill(sign1);
	}
}
