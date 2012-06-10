package modelo.atributoGraficable;

import java.awt.Point;
import java.awt.geom.Line2D;

import javax.swing.JLabel;
import javax.swing.JPanel;

public class AtributoGraficable extends JPanel {

	private static final long serialVersionUID = 1L;
	private Point inicio;
	private Point fin;
	
	public AtributoGraficable (Point puntoInicio, Point puntoFin) {
		this.inicio = puntoInicio;
		this.fin = puntoFin;
	}
	
	public AtributoGraficable(String name, int x, int y) {
		JLabel label = new JLabel(name);
		this.setLocation(x, y);
		this.add(label);

	}
	
}
