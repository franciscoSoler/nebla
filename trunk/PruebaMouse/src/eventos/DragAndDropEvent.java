package eventos;

import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import modelo.entidadGraficable.EntidadGraficable;

public class DragAndDropEvent implements MouseMotionListener, MouseListener {

	//private AreaDeDibujo areaDeDibujo;
	private EntidadGraficable entidadAMover;
	private Point puntoOriginal;

	public DragAndDropEvent(EntidadGraficable entidad) {
		//this.areaDeDibujo = area;
		this.entidadAMover = entidad;
	
	}
	
	@Override
	public void mouseDragged(MouseEvent me) {
			
		Point puntoAMover = new Point (this.entidadAMover.getLocation().x + me.getPoint().x-puntoOriginal.x ,this.entidadAMover.getLocation().y + me.getPoint().y-puntoOriginal.y);
		this.entidadAMover.mover(puntoAMover);
		
	}

	@Override
	public void mouseMoved(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseClicked(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseEntered(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseExited(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mousePressed(MouseEvent arg0) {
		// TODO Auto-generated method stub
		this.puntoOriginal = new Point(arg0.getPoint());
		
	}

	@Override
	public void mouseReleased(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
	}

}
