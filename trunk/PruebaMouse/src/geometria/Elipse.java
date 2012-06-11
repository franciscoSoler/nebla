package geometria;

import java.awt.Point;
import java.lang.Math;

public class Elipse {

	private Point center;
		
	private int alturaRenglon;
	private int cantidadPuntos;
	
	private double b;
	private double a;
	
	private double anguloMinimo;
	
	public Elipse (int altura, int cantidad) {
		this.alturaRenglon = altura;
		this.cantidadPuntos = cantidad;
		this.anguloMinimo = 2*Math.PI/this.cantidadPuntos;
		b = (this.alturaRenglon/2.0 + this.alturaRenglon*Math.ceil((this.cantidadPuntos - 4.0)/4));
		a = 2*b;
		
		System.out.println("B: "+b);
		System.out.println("A: "+a);
	}
	
	public double getA () {
		return this.a;
	}
	
	public double getB () {
		return this.b;
	}
	
	public Point getPunto(Point center, int n) {
		double angulo = n * this.anguloMinimo - Math.PI/2;
		Point punto = new Point(center.x+(int)(this.a * Math.cos(angulo)), center.y+(int)(this.b*Math.sin(angulo)));
		return punto;
	}
	
}
