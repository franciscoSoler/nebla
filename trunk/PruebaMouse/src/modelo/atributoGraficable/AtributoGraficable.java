package modelo.atributoGraficable;

import java.awt.Point;

import javax.swing.JPanel;

public class AtributoGraficable extends JPanel {


	private static final long serialVersionUID = 1L;
	private Point inicio;
	private Point fin;
	
	public AtributoGraficable (Point puntoInicio, Point puntoFin) {
		this.inicio = puntoInicio;
		this.fin = puntoFin;
		
		
	}
	
}
