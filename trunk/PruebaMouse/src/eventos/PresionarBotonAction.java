package eventos;

import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.KeyStroke;

import modelo.entidadGraficable.EntidadGraficable;

import actions.EscPressedAction;

import vista.paneles.AreaDeDibujo;

public class PresionarBotonAction implements ActionListener {

	private AreaDeDibujo areaDeDibujo;
	
	public PresionarBotonAction (AreaDeDibujo area) {
		this.areaDeDibujo = area;
		System.out.println("Contructor PresionarBoton");
	}
	
	public void actionPerformed(ActionEvent a) {
		System.out.println("Action performed");
				
		int x = areaDeDibujo.getX() + areaDeDibujo.getWidth()/2;
		int y = areaDeDibujo.getY() + areaDeDibujo.getHeight()/2;
		System.out.println("X: "+x);
		System.out.println("Y: "+y);
		
		EntidadGraficable entidad = new EntidadGraficable(new Rectangle(15,15, 150, 150));
		
		SeguirMouseEvent newEvent = new SeguirMouseEvent(this.areaDeDibujo,entidad);
		this.areaDeDibujo.addMouseListener(newEvent);
		this.areaDeDibujo.addMouseMotionListener(newEvent);
		
		EscPressedAction pressedAction = new EscPressedAction();

		this.areaDeDibujo.getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke("a"),"pressed");
		this.areaDeDibujo.getActionMap().put("pressed", pressedAction);
	}
}
