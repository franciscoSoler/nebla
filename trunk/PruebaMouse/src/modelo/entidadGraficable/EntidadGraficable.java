package modelo.entidadGraficable;

import java.awt.Graphics;
import java.awt.Rectangle;
import java.util.LinkedList;
import java.util.Iterator;

import javax.swing.JLabel;
import javax.swing.JPanel;

import vista.circle.Circle;

public class EntidadGraficable extends JPanel {

	private LinkedList<String> atributos;
	private LinkedList<JLabel> atributosGraficables;
	
	private static final long serialVersionUID = 1L;
	
	public EntidadGraficable (Rectangle bounds){	
		JLabel label = new JLabel("Prueba 1");
		label.setBounds(new Rectangle(0,0,75,75));
		this.add(label);
		this.setBounds(bounds);
				
		atributos = new LinkedList<String>();
		atributosGraficables = new LinkedList<JLabel>();
		
		atributos.add("Atributo1");
		atributos.add("Atributo2");
		atributos.add("Atributo3");
		this.crearAtributosGraficables();
		
		Circle circulo = new Circle(10, 20, 20);
		this.add(circulo);
	}
	
	private void crearAtributosGraficables() {
		Iterator<String> iterator = atributos.iterator();
		while (iterator.hasNext()) {
			JLabel label = new JLabel(iterator.next());
			label.setBounds(new Rectangle(0,0,75,75));
			
			atributosGraficables.add(label);
			
		}
	}

}