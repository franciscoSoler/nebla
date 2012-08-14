package vista.paneles;

import java.util.LinkedList;

import javax.swing.JComponent;
import javax.swing.JPanel;

public class AreaDeDibujo extends JPanel {

	private static final long serialVersionUID = 1L;
	private LinkedList<JComponent> componentes; 
	
	public AreaDeDibujo () {
		this.componentes = new LinkedList<JComponent>();
	}
	 
	public void agregarComopnente (JComponent componente) {
		this.componentes.add(componente);
		System.out.println("Cantidad: "+this.componentes.size());
	}
	
	public LinkedList<JComponent> obtenerComponentes () {
		return this.componentes;
	}
	
	

	
	

	

}
