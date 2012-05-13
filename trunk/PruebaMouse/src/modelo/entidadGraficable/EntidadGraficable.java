package modelo.entidadGraficable;

import java.awt.Rectangle;

import javax.swing.JLabel;
import javax.swing.JPanel;

import modelo.atributoGraficable.AtributoGraficable;

public class EntidadGraficable extends JPanel {

	private static final long serialVersionUID = 1L;
	
	public EntidadGraficable (Rectangle bounds){	
		JLabel label = new JLabel("Prueba 1");
		label.setBounds(bounds);
		label.setBounds(new Rectangle(0,0,75,75));
		this.add(label);
		this.setBounds(bounds);
	}
	
	public void setAtributo(String atrName) {
		AtributoGraficable atributo;
	}
}