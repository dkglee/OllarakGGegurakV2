// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceRecorderComponent.h"

#include "AudioMixerBlueprintLibrary.h"
#include "AudioMixerDevice.h"
#include "JumpGame/Utils/FastLogger.h"

bool UVoiceRecorderComponent::Init(int32& SampleRate)
{
	NumChannels = 2;

#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Initialize the DSP objects
	Osc.Init(SampleRate);
	Osc.SetFrequency(440.0f);
	Osc.Start();
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	

	if (RecordSoundSubmix)
	{
		this->SoundSubmix = RecordSoundSubmix;
	}
	
	return true;
}

int32 UVoiceRecorderComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Perform DSP operations here
	for (int32 Sample = 0; Sample < NumSamples; ++Sample)
	{
		OutAudio[Sample] = Osc.Generate();
	}
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED

	return Super::OnGenerateAudio(OutAudio, NumSamples);
}

void UVoiceRecorderComponent::SetFrequency(const float InFrequencyHz)
{
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
	{
		Osc.SetFrequency(InFrequencyHz);
		Osc.Update();
	});
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
}

void UVoiceRecorderComponent::StartRecording()
{
	this->Start();
	UAudioMixerBlueprintLibrary::StartRecordingOutput(GetWorld(), 0.0f, RecordSoundSubmix.Get());
	FFastLogger::LogConsole(TEXT("녹음 시작했습니다"));

	OnStartRecording.Broadcast();
}
void UVoiceRecorderComponent::StopRecording(const bool bIsStop)
{
	if (!IsRecording()) return;

	if (!bIsStop)
	{
		// 상대 경로를 절대 경로로 변경해줘야 함.
		FString FiledirectoryPath = FPaths::ProjectDir() + TEXT("Saved/AudioRecordings/");
		if (!FPaths::DirectoryExists(FiledirectoryPath))
		{
			// 디렉토리 없으면 생성
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			if (!PlatformFile.DirectoryExists(*FiledirectoryPath))
			{
				PlatformFile.CreateDirectoryTree(*FiledirectoryPath);
			}
		}
		FString AbsolutePath = FPaths::ConvertRelativePathToFull(FiledirectoryPath);
		
		Audio::FMixerDevice* MixerDevice = FAudioDeviceManager::GetAudioMixerDeviceFromWorldContext(GetWorld());
		if (MixerDevice)
		{
			float OutSampleRate = 0.0f;
			float OutChannelCount = 0.0f;

			Audio::FAlignedFloatBuffer& RecordedBuffer = MixerDevice->StopRecording(RecordSoundSubmix.Get(), OutChannelCount, OutSampleRate);

			if (RecordedBuffer.Num() == 0)
			{
				return;
			}

			RecordingData.Reset(new Audio::FAudioRecordingData());
			RecordingData->InputBuffer = Audio::TSampleBuffer<int16>(RecordedBuffer, OutChannelCount, OutSampleRate);

			RecordingData->Writer.BeginWriteToWavFile(RecordingData->InputBuffer, TEXT("SoundQuizResponseFile"), AbsolutePath, [this]()
			{
				if (RecordSoundSubmix && RecordSoundSubmix->OnSubmixRecordedFileDone.IsBound())
				{
					RecordSoundSubmix->OnSubmixRecordedFileDone.Broadcast(nullptr);
				}
				RecordingData.Reset();
			});
		}
		else
		{
			FFastLogger::LogConsole(TEXT("nullptr"));
		}
	}
	this->Stop();
	OnStopRecording.Broadcast();
}

const bool UVoiceRecorderComponent::IsRecording() const
{
	return IsPlaying();
}

