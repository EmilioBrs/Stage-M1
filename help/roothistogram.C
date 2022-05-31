void roothistogram()//doit avoir le même nom que le titre du fichier
{

	TH1F *hist = new TH1F("hist", "Histogram", 1000, 0 , 1000);
	//H = histogram, 1 = 1 dimension, F = float (to put in float values)
	//100= nombre de valeurs, 0 = start value, 100 = end value

	ifstream f ("datadistances.txt");
	double value;
	
	while(f.eof()==false)
	{
		file>>value;
		hist->Fill(value);
	}
	f.close();
	//on met des valeurs dans l'histogramme

	hist->GetXaxis()->SetTitle("TitreaxeX");
	hist->GetYaxis()->SetTitle("TitreaxeY");
	//titre des axes

	TCanvas *c1 = new TCanvas();
	//pour tracer l'histogramme (fenêtre dans laquelle il va être tracé)
	hist->Draw();
	c1->SaveAs("Histogramme_distances.pdf");
} 
