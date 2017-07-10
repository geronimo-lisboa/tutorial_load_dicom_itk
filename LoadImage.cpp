#include <string>
#include <cstdlib>

#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
//NEW - The observer
#include <itkCommand.h>
//Typedefs
typedef itk::Image<short, 3> ImageType;
typedef itk::ImageSeriesReader<ImageType> ReaderType;
typedef itk::GDCMImageIO ImageIOType;
typedef itk::GDCMSeriesFileNames NamesGeneratorType;
typedef std::vector< std::string >    SeriesIdContainer;
typedef std::vector< std::string >   FileNamesContainer;

namespace itk
{
	class myProgressObserver:public Command
	{
	public:
		//ITK macro - takes care of defining the static New() that all
		//itk object must have to be compatible with ITK's memory manegement.
		itkNewMacro(myProgressObserver);

		void Execute(itk::Object *caller, const itk::EventObject & event)
		{
			Execute((const itk::Object *)caller, event);
		}

		void Execute(const itk::Object * object, const itk::EventObject & event)
		{
			const itk::ProcessObject  *processObj = dynamic_cast<const itk::ProcessObject*>(object);
			std::cout << processObj->GetProgress() << std::endl;
		}
	};
}

int main(int argc, char** argv)
{
	////Set which imageIO will my reader use. The actual IO operation is encapsulated
	////in classes like GDCMImageIO, so that the specifics of each file format be well 
	////isolated.
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer imageIOObject = ImageIOType::New();
	reader->SetImageIO(imageIOObject);
	//NEW - THE OBSERVER
	itk::myProgressObserver::Pointer obs = itk::myProgressObserver::New();
	
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	//NEW - Registers the observer
	nameGenerator->AddObserver(itk::ProgressEvent(), obs);
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->SetDirectory("C:\\Users\\geronimo\\dicom\\Visible Human Male CT DICOM");
	//This list has all the images in the directory
	SeriesIdContainer seriesUID = nameGenerator->GetSeriesUIDs();
	
	//The files that contains each of the individual slices that define an image.
	FileNamesContainer fileNames = nameGenerator->GetFileNames(seriesUID[0]);
	reader->SetFileNames(fileNames);
	try
	{
		reader->AddObserver(itk::ProgressEvent(), obs);
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