package modelo.entidadGraficable;

import geometria.Elipse;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.util.Iterator;
import java.util.LinkedList;

import javax.swing.JLabel;
import javax.swing.JPanel;



public class EntidadGraficable extends JPanel {

	private JLabel labelNombre;
	
	private String nombreentidad; 
	
	private int radioCirculo = 10;
	private int alturaLabel = 25;
		
	private JPanel areaDeDibujo;
	
	private LinkedList<String> atributos;
	
	private Elipse elipse;
	
	private static final long serialVersionUID = 1L;
	
	public EntidadGraficable (String nombre, JPanel areaDeDibujo, Point origin){	
		this.nombreentidad = nombre;
		this.setLayout(null);
		this.areaDeDibujo = areaDeDibujo;
		this.setBackground(Color.red);
		
		atributos = new LinkedList<String>();
		atributos.add("Atributo0");
		atributos.add("Atributo1");
		atributos.add("Atributo2");
		atributos.add("Atributo3");
		atributos.add("Atributo4");
		atributos.add("Atributo5");
		atributos.add("Atributo6");
		atributos.add("Atributo7");
	
		elipse = new Elipse(this.alturaLabel, atributos.size());
		int a = (int)elipse.getA();
		int b = (int)elipse.getB();
		this.setBounds(origin.x, origin.y, 2*a+radioCirculo, 2*b+radioCirculo);
		
		labelNombre = new JLabel(this.nombreentidad);
		labelNombre.setSize(labelNombre.getMinimumSize());
		labelNombre.setLocation(this.getSize().width/2 - labelNombre.getSize().width/2, this.getSize().height/2 - labelNombre.getSize().height/2);
		this.add(labelNombre);
		this.updateUI();
		
		this.updateUI();
		this.areaDeDibujo.add(this);
		this.areaDeDibujo.updateUI();
	}
	
	public void mover(Point origin) {
		this.setLocation(origin);
	}
	
	public void paint(Graphics g) {
		super.paint(g);

		Point centro = new Point(this.getWidth()/2,this.getHeight()/2);

		Iterator<String> iterator = atributos.iterator();
		int i = 0;
		while (iterator.hasNext()) {
			Point punto = elipse.getPunto(centro, i++);
			g.fillOval(punto.x-radioCirculo/2, punto.y-radioCirculo/2, radioCirculo, radioCirculo);
			g.drawLine(centro.x, centro.y, punto.x, punto.y);
			
			String nombreAtributo = iterator.next();
		}
		
		this.add(labelNombre);
		this.updateUI();

	}
}