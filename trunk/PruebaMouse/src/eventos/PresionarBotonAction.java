package eventos;

import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import modelo.entidadGraficable.EntidadGraficable;
import vista.paneles.AreaDeDibujo;

public class PresionarBotonAction implements ActionListener {

	private int cantidad;
	private AreaDeDibujo areaDeDibujo;
	
	public PresionarBotonAction (AreaDeDibujo area) {
		this.areaDeDibujo = area;
		this.cantidad = 0;
		//System.out.println("Contructor PresionarBoton");
	}
	
	public void actionPerformed(ActionEvent a) {
		//System.out.println("Action performed");
				
		//int x = areaDeDibujo.getX() + areaDeDibujo.getWidth()/2;
		//int y = areaDeDibujo.getY() + areaDeDibujo.getHeight()/2;
		//System.out.println("X: "+x);
		//System.out.println("Y: "+y);
		
		String nombre = "Entidad " + this.cantidad++;
		EntidadGraficable entidad = new EntidadGraficable(nombre,this.areaDeDibujo, new Point(15,15));
		
		//SeguirMouseEvent newEvent = new SeguirMouseEvent(entidad);
		//this.areaDeDibujo.addMouseListener(newEvent);
		//this.areaDeDibujo.addMouseMotionListener(newEvent);
		
		//EscPressedAction pressedAction = new EscPressedAction();
		//this.areaDeDibujo.getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke("a"),"pressed");
		//this.areaDeDibujo.getActionMap().put("pressed", pressedAction);
	}
}
