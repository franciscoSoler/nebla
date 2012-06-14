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

	Point centroActual;// = new Point(this.getWidth()/2,this.getHeight()/2);

	private String nombreentidad; 
	
	private int radioCirculo = 10;
	private int alturaLabel = 25;
		
	private JPanel areaDeDibujo;
	
	private LinkedList<String> atributos;
	private LinkedList<JLabel> labelAtributos;
	
	private Elipse elipse;
	
	private Point posicionActual;
	
	private static final long serialVersionUID = 1L;
	
	public EntidadGraficable (String nombre, JPanel areaDeDibujo, Point origin){	
		this.nombreentidad = nombre;
		this.setLayout(null);
		this.areaDeDibujo = areaDeDibujo;
		this.setBackground(Color.red);
		
		labelAtributos = new LinkedList<JLabel>();
		atributos = new LinkedList<String>();
		
		atributos.add("Atributo 0");
		atributos.add("Atributo numero 1");
		atributos.add("Atributo 2");
		atributos.add("Atributo 3");
		atributos.add("Atributo 4");
		atributos.add("Atributo 5");
		atributos.add("Atributo 6");
		atributos.add("Atributo 7");
		
		Iterator<String> iteratorNombre = atributos.iterator();
		
		while (iteratorNombre.hasNext()) {
			JLabel labelAAgregar = new JLabel(iteratorNombre.next());
			labelAAgregar.setSize(labelAAgregar.getMinimumSize().width,this.alturaLabel);
			labelAtributos.add(labelAAgregar);
		}
		
		elipse = new Elipse(this.alturaLabel, atributos.size());
		int a = (int)elipse.getA();
		int b = (int)elipse.getB();
		this.setBounds(origin.x, origin.y, 2*a+radioCirculo, 2*b+radioCirculo + 2*this.alturaLabel);
		this.posicionActual = new Point(origin);
		Point centro = new Point(this.getWidth()/2,this.getHeight()/2);
		
		int i = 0;
		Iterator<JLabel> iteratorLabel = labelAtributos.iterator();
		
		int xMax = 0;
		int xMin = 0;	
		while (iteratorLabel.hasNext()) {
			Point punto = elipse.getPunto(centro, i++);
			JLabel labelAtributo = iteratorLabel.next();
			
			int y = punto.y - this.alturaLabel/2;
			labelAtributo.setVerticalAlignment(JLabel.CENTER);
			
			if (punto.x > centro.x) {
				labelAtributo.setLocation(punto.x + this.radioCirculo/2, y);
				int ancho = labelAtributo.getSize().width + labelAtributo.getLocation().x;
				if (ancho > xMax) {
					xMax = ancho;
					System.out.println("Nombre: "+labelAtributo.getText() +"xMax: "+xMax);
				}
				
			}
			else if (punto.x < centro.x) {
				labelAtributo.setLocation(punto.x - this.radioCirculo/2 - labelAtributo.getSize().width, y);
				if (labelAtributo.getLocation().x < xMin) {
					xMin = labelAtributo.getLocation().x;
				}
			}
			else {
				if (punto.y < centro.y) {
					labelAtributo.setVerticalAlignment(JLabel.BOTTOM);
					labelAtributo.setLocation(punto.x - labelAtributo.getSize().width/2, punto.y - this.radioCirculo/2 - this.alturaLabel);
				}
				else {
					labelAtributo.setVerticalAlignment(JLabel.TOP);
					labelAtributo.setLocation(punto.x - labelAtributo.getSize().width/2, punto.y + this.radioCirculo/2 );					
				}
			}
			
			this.add(labelAtributo);
		}
		
		this.setSize(xMax+Math.abs(xMin), this.getSize().height);
		
		this.centroActual = new Point(centro.x+Math.abs(xMin),centro.y);
		
		iteratorLabel = labelAtributos.iterator();
		//Point centroNuevo = new Point(this.getSize().width/2, this.getSize().height/2);
		while (iteratorLabel.hasNext()) {
			//int diff = centroNuevo.x - centro.x; 
			JLabel labelAtributo = iteratorLabel.next();
			labelAtributo.setLocation(labelAtributo.getLocation().x+Math.abs(xMin), labelAtributo.getLocation().y);
		}
		
		JLabel labelNombre = new JLabel(this.nombreentidad);
		labelNombre.setSize(labelNombre.getMinimumSize());
		labelNombre.setLocation(this.centroActual.x - labelNombre.getSize().width/2, this.centroActual.y - labelNombre.getSize().height/2);
		this.add(labelNombre);
		
		this.areaDeDibujo.add(this);
		this.areaDeDibujo.updateUI();
	}
	
	public void mover(Point origin) {
		if ((origin.x > 0) && (origin.y > 0) && (origin.x+this.getSize().width < this.areaDeDibujo.getSize().width) && (origin.y+this.getSize().height < this.areaDeDibujo.getSize().height)) {
			System.out.println("Se deberia estar moviendo");
			this.setLocation(origin);
		}
	}
	
	public void paint(Graphics g) {
		super.paint(g);

		Iterator<JLabel> iterator = labelAtributos.iterator();
		int i = 0;
		while (iterator.hasNext()) {
			Point punto = elipse.getPunto(this.centroActual, i++);
			g.fillOval(punto.x-radioCirculo/2, punto.y-radioCirculo/2, radioCirculo, radioCirculo);
			g.drawLine(this.centroActual.x, this.centroActual.y, punto.x, punto.y);
			iterator.next();
		}
	}
}