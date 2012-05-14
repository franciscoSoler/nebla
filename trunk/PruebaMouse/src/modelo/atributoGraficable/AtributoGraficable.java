package modelo.atributoGraficable;

import java.awt.Point;
import java.awt.geom.Line2D;

import javax.swing.JPanel;

public class AtributoGraficable extends JPanel {


	private static final long serialVersionUID = 1L;
	private Point inicio;
	private Point fin;
	
	public AtributoGraficable (Point puntoInicio, Point puntoFin) {
		this.inicio = puntoInicio;
		this.fin = puntoFin;
		
		Line2D line = new Line2D.Float();
	}
	
}
