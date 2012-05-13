package vista.paneles;

import javax.swing.JButton;
import javax.swing.JPanel;

import eventos.PresionarBotonAction;

public class AreaDeDibujo extends JPanel {

	private static final long serialVersionUID = 1L;
	
	public AreaDeDibujo () {
		JButton button = new JButton("Boton START");
		button.setBounds(10, 150, 150, 100);
		button.addActionListener(new PresionarBotonAction(this));
		this.add(button);
	}

	
	

	

}
