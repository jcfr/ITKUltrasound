/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkSpectra1DSupportWindowToMaskImageFilter_hxx
#define itkSpectra1DSupportWindowToMaskImageFilter_hxx

#include "itkSpectra1DSupportWindowImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkMetaDataObject.h"

namespace itk
{

template <typename TInputImage, typename TOutputImage>
Spectra1DSupportWindowToMaskImageFilter<TInputImage, TOutputImage>::Spectra1DSupportWindowToMaskImageFilter()
  : m_BackgroundValue(NumericTraits<OutputPixelType>::ZeroValue())
  , m_ForegroundValue(NumericTraits<OutputPixelType>::max())
{
  m_MaskIndex.Fill(0);
}


template <typename TInputImage, typename TOutputImage>
void
Spectra1DSupportWindowToMaskImageFilter<TInputImage, TOutputImage>::GenerateData()
{
  this->AllocateOutputs();

  const InputImageType * input = this->GetInput();
  using InputPixelType = typename InputImageType::PixelType;
  const InputPixelType & inputPixel = input->GetPixel(this->GetMaskIndex());

  using Spectra1DSupportWindowFilterType = Spectra1DSupportWindowImageFilter<OutputImageType>;
  using FFT1DSizeType = typename Spectra1DSupportWindowFilterType::FFT1DSizeType;

  const MetaDataDictionary & dict = input->GetMetaDataDictionary();
  FFT1DSizeType              fft1DSize = 32;
  ExposeMetaData<FFT1DSizeType>(dict, "FFT1DSize", fft1DSize);

  OutputImageType * output = this->GetOutput();
  output->FillBuffer(this->GetBackgroundValue());

  for (typename InputPixelType::const_iterator lineIt = inputPixel.begin(); lineIt != inputPixel.end(); ++lineIt)
  {
    const IndexType startIndex = *lineIt;
    IndexType       index = startIndex;
    for (FFT1DSizeType sampleIndex = 0; sampleIndex < fft1DSize; ++sampleIndex)
    {
      index[0] = startIndex[0] + sampleIndex;
      output->SetPixel(index, this->GetForegroundValue());
    }
  }
}

} // end namespace itk

#endif
