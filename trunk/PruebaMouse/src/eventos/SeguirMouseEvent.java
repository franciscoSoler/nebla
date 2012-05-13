package eventos;

import java.awt.event.MouseEvent;

import javax.swing.JLabel;
import javax.swing.event.MouseInputListener;

import vista.paneles.AreaDeDibujo;

public class SeguirMouseEvent implements MouseInputListener {

	private AreaDeDibujo areaDeDibujo;
	private JLabel labelAMover;
	public SeguirMouseEvent(AreaDeDibujo area, JLabel label) {
		this.areaDeDibujo = area;
		this.labelAMover = label;
		this.areaDeDibujo.add(labelAMover);
		System.out.println("Contructor Anclar mouse event");
		
	}
	
	public void mouseClicked(MouseEvent e) {
		System.out.println("Mouse clicked on Area De Dibujo");
		
		areaDeDibujo.removeMouseListener(this);
		areaDeDibujo.removeMouseMotionListener(this);
		
	}
	
	public void mouseMoved(MouseEvent e) {
		System.out.println("Moviendo el mouse");
		System.out.println(e.getX());
		System.out.println(e.getY());
		
		this.labelAMover.setLocation(e.getPoint());
		
	}
	
	public void mouseEntered (MouseEvent e) {
		System.out.println("Mouse entrando en el panel");
		System.out.println(e.getX());
		System.out.println(e.getY());
	}
	
	public void mouseExited (MouseEvent e) {
		System.out.println("Mouse saliendo del panel");
		System.out.println(e.getX());
		System.out.println(e.getY());
	}

	@Override
	public void mousePressed(MouseEvent e) {
		// TODO Auto-generated method stub		
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		// TODO Auto-generated method stub
	}
	
	

}
