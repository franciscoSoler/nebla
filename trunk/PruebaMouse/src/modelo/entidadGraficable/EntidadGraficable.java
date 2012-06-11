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

	private int radioCirculo = 10;
	private int alturaLabel = 25;
		
	private JPanel areaDeDibujo;
	
	private LinkedList<String> atributos;
	private LinkedList<JLabel> atributosGraficable;
	
	private Elipse elipse;
	
	private static final long serialVersionUID = 1L;
	
	public EntidadGraficable (String nombre, JPanel areaDeDibujo, Point origin){	
		this.setLayout(null);
		this.areaDeDibujo = areaDeDibujo;
		this.setBackground(Color.red);
		
		atributosGraficable = new LinkedList<JLabel>();
		atributos = new LinkedList<String>();
		atributos.add("Atributo0");
		atributos.add("Atributo1");
		atributos.add("Atributo2");
		atributos.add("Atributo3");
		atributos.add("Atributo4");
		atributos.add("Atributo5");
		atributos.add("Atributo6");
		atributos.add("Atributo7");
		atributos.add("Atributo8");
	
		elipse = new Elipse(this.alturaLabel, atributos.size());
		int a = (int)elipse.getA();
		int b = (int)elipse.getB();
		this.setBounds(origin.x, origin.y, 2*a+radioCirculo, 2*b+radioCirculo);
		
		JLabel label = new JLabel(nombre);
		label.setBackground(Color.red);
		label.setSize(label.getMinimumSize());
		label.setLocation(this.getSize().width/2 - label.getSize().width/2, this.getSize().height/2 - label.getSize().height/2);
		
		this.add(label);
		
		this.updateUI();
		this.areaDeDibujo.add(this);
		this.areaDeDibujo.updateUI();
	}
	
	@SuppressWarnings("unused")
	private void crearAtributosGraficables() {
		
		//public Elipse (Point center, int altura, int cantidad)
		
		/*Iterator<String> iterator = atributos.iterator();
		int i = 0;
		while (iterator.hasNext()) {
			JLabel label = new JLabel(iterator.next());
			int width = label.getMinimumSize().width;
			int height = label.getMinimumSize().height;
			
			label.setBounds(new Rectangle(0,0,label.getMinimumSize().width,label.getMinimumSize().height));
		}*/
	}
	
	public void mover(Point origin) {
		this.setLocation(origin);
	}
	
	public void paint(Graphics g) {
		super.paint(g);
		System.out.println("Painting graphics 2D");
		
			
		Point centro = new Point(this.getWidth()/2,this.getHeight()/2);
		//public Elipse (Point center, int altura, int cantidad)
		
		Iterator<String> iterator = atributos.iterator();
		int i = 0;
		while (iterator.hasNext()) {
			Point punto = elipse.getPunto(centro, i++);
			g.fillOval(punto.x-radioCirculo/2, punto.y-radioCirculo/2, radioCirculo, radioCirculo);
			g.drawLine(centro.x, centro.y, punto.x, punto.y);
			iterator.next();
		}
	}

}