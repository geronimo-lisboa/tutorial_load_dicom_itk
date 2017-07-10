#include <string>

#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
//Typedefs
typedef itk::Image<short, 3> ImageType;
typedef itk::ImageSeriesReader<ImageType> ReaderType;
typedef itk::GDCMImageIO ImageIOType;
typedef itk::GDCMSeriesFileNames NamesGeneratorType;
typedef std::vector< std::string >    SeriesIdContainer;
typedef std::vector< std::string >   FileNamesContainer;

int main(int argc, char** argv)
{
	////Set which imageIO will my reader use. The actual IO operation is encapsulated
	////in classes like GDCMImageIO, so that the specifics of each file format be well 
	////isolated.
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer imageIOObject = ImageIOType::New();
	reader->SetImageIO(imageIOObject);
	
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->SetDirectory("C:\\meus dicoms\\Visible Human Male CT DICOM");
	//This list has all the images in the directory
	SeriesIdContainer seriesUID = nameGenerator->GetSeriesUIDs();
	
	//The files that contains each of the individual slices that define an image.
	FileNamesContainer fileNames = nameGenerator->GetFileNames(seriesUID[0]);
	reader->SetFileNames(fileNames);
	try
	{
		reader->Update();
		ImageType::Pointer result = reader->GetOutput();
		result->Print(std::cout);//My image, loaded.
	}
	catch (itk::ExceptionObject& ex)
	{
		ex.Print(std::cout);
	}
	return 0;
}