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


	private String nombreentidad; 
	
	private int radioCirculo = 10;
	private int alturaLabel = 25;
		
	private JPanel areaDeDibujo;
	
	private LinkedList<String> atributos;
	private LinkedList<JLabel> labelAtributos;
	
	private Elipse elipse;
	
	private static final long serialVersionUID = 1L;
	
	public EntidadGraficable (String nombre, JPanel areaDeDibujo, Point origin){	
		this.nombreentidad = nombre;
		this.setLayout(null);
		this.areaDeDibujo = areaDeDibujo;
		this.setBackground(Color.red);
		
		labelAtributos = new LinkedList<JLabel>();
		atributos = new LinkedList<String>();
		
		atributos.add("Atributo0");
		atributos.add("Atributo1");
		/*atributos.add("Atributo2");
		atributos.add("Atributo3");
		atributos.add("Atributo4");
		atributos.add("Atributo5");
		atributos.add("Atributo6");
		atributos.add("Atributo7");*/
		
		Iterator<String> iterator = atributos.iterator();
		int i = 1;
		while (iterator.hasNext()) {
			JLabel labelAAgregar = new JLabel(iterator.next());
			labelAAgregar.setSize(labelAAgregar.getMinimumSize());
			//labelAAgregar.setVerticalTextPosition(JLabel.CENTER);
			labelAtributos.add(labelAAgregar);
			this.add(labelAAgregar);
			System.out.println("Cantidad: "+i++);
		}
	
		elipse = new Elipse(this.alturaLabel, atributos.size());
		int a = (int)elipse.getA();
		int b = (int)elipse.getB();
		this.setBounds(origin.x, origin.y, 2*a+radioCirculo, 2*b+radioCirculo + 2*this.alturaLabel);
		
		JLabel labelNombre = new JLabel(this.nombreentidad);
		labelNombre.setSize(labelNombre.getMinimumSize());
		labelNombre.setLocation(this.getSize().width/2 - labelNombre.getSize().width/2, this.getSize().height/2 - labelNombre.getSize().height/2);
		this.add(labelNombre);
		
		this.areaDeDibujo.add(this);
		this.areaDeDibujo.updateUI();
	}
	
	public void mover(Point origin) {
		this.setLocation(origin);
	}
	
	public void paint(Graphics g) {
		super.paint(g);

		Point centro = new Point(this.getWidth()/2,this.getHeight()/2);

		Iterator<JLabel> iterator = labelAtributos.iterator();
		int i = 0;
		while (iterator.hasNext()) {
			Point punto = elipse.getPunto(centro, i++);
			g.fillOval(punto.x-radioCirculo/2, punto.y-radioCirculo/2, radioCirculo, radioCirculo);
			g.drawLine(centro.x, centro.y, punto.x, punto.y);

			JLabel labelAtributo = iterator.next();
			
			if (punto.x > centro.x) {
				if (punto.y <= centro.y) {
					// Primer cuadrante: 0 <= angulo <= 90
					
				}
				else {
					// Segundo cuadrante: 90 < angulo <= 180
					
				}
			}
			else if (punto.x < centro.x) {
				if (punto.y >= centro.y) {
					// Tercer cuadrante: 180 < angulo <= 270
					
				}
				else {
					// Cuarto cuadrante: 270 < angulo < 360
					
				}
			}
			else {
				if (punto.y < centro.y) {
					System.out.println("Entro aca 1");
					labelAtributo.setLocation(punto.x - labelAtributo.getSize().width/2, punto.y - this.radioCirculo/2 - this.alturaLabel);
				}
				else {
					System.out.println("Entro aca 2");
					labelAtributo.setLocation(punto.x - labelAtributo.getSize().width/2, punto.y + this.radioCirculo/2 + this.alturaLabel);
				}
			}
		}
	}
}