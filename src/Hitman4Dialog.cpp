//
// Created by Andrej Redeky.
// Copyright © 2015-2023 Feldarian Softworks. All rights reserved.
// SPDX-License-Identifier: EUPL-1.2
//

#include "Precompiled.hpp"

#include "Hitman4Dialog.hpp"

HitmanSoundRecord Hitman4WHDRecord::Hitman4WHDRecordMission::ToHitmanSoundRecord() const
{
  assert(filePathLength > 0 && filePathLength < 0x004F4850);

  const auto resBitsPerSample = static_cast<uint16_t>(formatTag == 4096 ? 16 : bitsPerSample);
  const auto resChannels = static_cast<uint16_t>(channels);
  const auto resBlockAlign = static_cast<uint16_t>(formatTag == 4096 ? 2 * channels : blockAlign);
  const auto resFmtExtra = static_cast<uint16_t>(formatTag == 17 ? fmtExtra : 1);

  assert(resBlockAlign == 2 * channels || formatTag == 17);
  assert(resBlockAlign != 1024 || formatTag == 17);

  return {
    0,
    dataSizeUncompressed,
    dataSize,
    sampleRate,
    formatTag,
    resBitsPerSample,
    resChannels,
    resBlockAlign,
    resFmtExtra
  };
}

void Hitman4WHDRecord::Hitman4WHDRecordMission::FromHitmanSoundRecord(const HitmanSoundRecord &soundRecord)
{
  assert(formatTag != 4096 || formatTag == soundRecord.formatTag);

  formatTag = soundRecord.formatTag;
  sampleRate = soundRecord.sampleRate;
  bitsPerSample = formatTag == 4096 ? 0 : soundRecord.bitsPerSample;
  dataSizeUncompressed = soundRecord.dataSizeUncompressed;
  dataSize = soundRecord.dataSize;
  channels = soundRecord.channels;
  samplesCount = soundRecord.dataSizeUncompressed / sizeof(int16_t);
  blockAlign = formatTag == 4096 ? blockAlign : soundRecord.blockAlign;
  fmtExtra = formatTag == 4096 ? 0x004F3E93 : (formatTag == 1 ? 0xCDCDCDCD : soundRecord.fmtExtra);
}

HitmanSoundRecord Hitman4WHDRecord::Hitman4WHDRecordConcatenated::ToHitmanSoundRecord() const
{
  assert(nullByte == 0);

  const auto resBitsPerSample = static_cast<uint16_t>(formatTag == 4096 ? 16 : bitsPerSample);
  const auto resChannels = static_cast<uint16_t>(channels);
  const auto resBlockAlign = static_cast<uint16_t>(formatTag == 4096 ? 2 * channels : blockAlign);
  const auto resFmtExtra = static_cast<uint16_t>(formatTag == 17 ? fmtExtra : 1);

  assert(resBlockAlign == 2 * channels || formatTag == 17);
  assert(resBlockAlign != 1024 || formatTag == 17);

  return {
    0,
    dataSizeUncompressed,
    dataSize,
    sampleRate,
    formatTag,
    resBitsPerSample,
    resChannels,
    resBlockAlign,
    resFmtExtra
  };
}

void Hitman4WHDRecord::Hitman4WHDRecordConcatenated::FromHitmanSoundRecord(const HitmanSoundRecord &soundRecord)
{
  assert(formatTag != 4096 || formatTag == soundRecord.formatTag);

  formatTag = soundRecord.formatTag;
  sampleRate = soundRecord.sampleRate;
  bitsPerSample = formatTag == 4096 ? 0 : soundRecord.bitsPerSample;
  dataSizeUncompressed = soundRecord.dataSizeUncompressed;
  dataSize = soundRecord.dataSize;
  channels = soundRecord.channels;
  samplesCount = soundRecord.dataSizeUncompressed / sizeof(int16_t);
  blockAlign = formatTag == 4096 ? blockAlign : soundRecord.blockAlign;
  fmtExtra = formatTag == 4096 ? 0x004F3E93 : (formatTag == 1 ? 0xCDCDCDCD : soundRecord.fmtExtra);
}

HitmanSoundRecord Hitman4WHDRecord::Hitman4WHDRecordStreams::ToHitmanSoundRecord() const
{
  assert(id == 0x004F4850);

  const auto resBitsPerSample = static_cast<uint16_t>(formatTag == 4096 ? 16 : bitsPerSample);
  const auto resChannels = static_cast<uint16_t>(channels);
  const auto resBlockAlign = static_cast<uint16_t>(formatTag == 4096 || formatTag == 1 ? 2 * channels : 1024);
  const auto resFmtExtra = static_cast<uint16_t>(formatTag == 17 ? fmtExtra : 1);

  assert(resBlockAlign == 2 * channels || formatTag == 17);
  assert(resBlockAlign != 1024 || formatTag == 17);

  return {
    0,
    dataSizeUncompressed,
    dataSize,
    unkC,
    formatTag,
    resBitsPerSample,
    resChannels,
    resBlockAlign,
    resFmtExtra
  };
}

void Hitman4WHDRecord::Hitman4WHDRecordStreams::FromHitmanSoundRecord(const HitmanSoundRecord &soundRecord)
{
  assert(formatTag == soundRecord.formatTag);

  formatTag = soundRecord.formatTag;
  unk2C = soundRecord.sampleRate;
  bitsPerSample = formatTag == 4096 ? 0 : soundRecord.bitsPerSample;
  dataSizeUncompressed = soundRecord.dataSizeUncompressed;
  dataSize = soundRecord.dataSize;
  channels = soundRecord.channels;
  samplesCount = soundRecord.dataSizeUncompressed / sizeof(int16_t);
  fmtExtra = formatTag == 4096 ? 0x004F3E93 : (formatTag == 1 ? 0xCDCDCDCD : soundRecord.fmtExtra);
}

HitmanSoundRecord Hitman4WHDRecord::ToHitmanSoundRecord() const
{
  assert((streams.id == 0x004F4850) == (streams.dataInStreams == 128));

  assert(streams.formatTag != 1 || streams.bitsPerSample == 16);
  assert(streams.formatTag != 17 || streams.bitsPerSample == 4);
  assert(streams.formatTag != 4096 || streams.bitsPerSample == 0);

  assert(mission.formatTag != 17 || mission.blockAlign == 1024);

  assert(streams.formatTag != 1 || streams.fmtExtra == 0xCDCDCDCD);
  assert(streams.formatTag != 17 || streams.fmtExtra == 2041);
  assert(streams.formatTag != 4096 || streams.fmtExtra == 0x004F3E93);

  assert(streams.unk2C == 0 || ((streams.formatTag == 17 || streams.formatTag == 1) && streams.id == 0x004F4850));

  assert(streams.nullBytes[0] == 0);
  assert(streams.nullBytes[1] == 0);
  assert(streams.nullBytes[2] == 0);

  switch (streams.id)
  {
    case 0x004F4850:
      return streams.ToHitmanSoundRecord();
    case 0:
      return concatenated.ToHitmanSoundRecord();
    default:
      return mission.ToHitmanSoundRecord();
  }
}

void Hitman4WHDRecord::FromHitmanSoundRecord(const HitmanSoundRecord &soundRecord)
{
  switch (streams.id)
  {
    case 0x004F4850:
      return streams.FromHitmanSoundRecord(soundRecord);
    case 0:
      return concatenated.FromHitmanSoundRecord(soundRecord);
    default:
      return mission.FromHitmanSoundRecord(soundRecord);
  }
}

bool Hitman4WAVFile::Clear(const bool retVal)
{
  header = nullptr;
  recordMap.clear();
  lipsData.clear();
  extraData.clear();
  path.clear();

  return retVal;
}

bool Hitman4WAVFile::Load(const StringView8CI loadPath, const OrderedMap<StringView8CI, Hitman4WHDRecord *> &whdRecordsMap, OrderedMap<StringView8CI, HitmanFile>& fileMap, const bool isMissionWAV)
{
  Clear();

  const auto wavData = ReadWholeBinaryFile(loadPath);
  if (wavData.empty())
    return Clear(false);

  struct WAVFileData
  {
    Hitman4WHDRecord *record = nullptr;
    HitmanFile* file = nullptr;
  };

  auto resampledOffset = static_cast<uint32_t>(wavData.size());
  size_t foundItems = 0;
  OrderedMap<uint32_t, uint32_t> resampledMap;
  OrderedMap<uint32_t, WAVFileData> offsetToWAVFileDataMap;
  for (auto& [whdRecordPath, whdRecord] : whdRecordsMap)
  {
    if ((whdRecord->streams.dataInStreams == 0) != isMissionWAV)
      continue;

    auto offsetToWAVFileDataIt = offsetToWAVFileDataMap.find(whdRecord->streams.dataOffset);
    if (offsetToWAVFileDataIt == offsetToWAVFileDataMap.end())
      offsetToWAVFileDataMap.insert({whdRecord->streams.dataOffset, {whdRecord, &fileMap[whdRecordPath]}});
    else
    {
      resampledMap[resampledOffset] = whdRecord->streams.dataOffset;
      whdRecord->streams.dataOffset = resampledOffset;
      resampledOffset += whdRecord->streams.dataSize;

      offsetToWAVFileDataMap.insert({whdRecord->streams.dataOffset, {whdRecord, &fileMap[whdRecordPath]}});
    }

    ++foundItems;
  }

  if (offsetToWAVFileDataMap.empty() || offsetToWAVFileDataMap.size() != foundItems)
    return true;

  extraData.reserve(2 * offsetToWAVFileDataMap.size() + 1);

  uint32_t currOffset = 0;
  for (auto& [offset, wavFileData] : offsetToWAVFileDataMap)
  {
    if (currOffset < offset)
    {
      auto& newData = extraData.emplace_back();
      newData.resize(offset - currOffset, 0);
      newData.shrink_to_fit();

      std::memcpy(newData.data(), wavData.data() + currOffset, offset - currOffset);
      recordMap.try_emplace(currOffset, Hitman4WAVRecord{newData, currOffset, std::nullopt});
    }
    assert(currOffset <= offset);

    auto& newData = wavFileData.file->data;
    newData.resize(wavFileData.record->streams.dataSize, 0);
    newData.shrink_to_fit();

    auto trueOffset = offset >= wavData.size() ? resampledMap[offset] : offset;
    const auto *wavDataLIP = reinterpret_cast<const Hitman4LIPData *>(wavData.data() + trueOffset);
    OptionalReference<std::vector<char>> lipData = std::nullopt;
    if (memcmp(wavDataLIP, "LIP ", sizeof(uint32_t)) == 0)
    {
      auto& emplacedLipData = lipsData.emplace_back();

      emplacedLipData.resize(0x1000, 0);
      emplacedLipData.shrink_to_fit();
      std::memcpy(emplacedLipData.data(), wavData.data() + trueOffset, emplacedLipData.size());

      lipData = emplacedLipData;
      trueOffset += static_cast<uint32_t>(emplacedLipData.size());
    }
    std::memcpy(newData.data(), wavData.data() + trueOffset, wavFileData.record->streams.dataSize);
    currOffset = offset + wavFileData.record->streams.dataSize + (lipData ? static_cast<uint32_t>(lipData->get().size()) : 0u);
    recordMap.try_emplace(offset, Hitman4WAVRecord{newData, offset, std::move(lipData)});
  }

  if (currOffset < wavData.size())
  {
    auto& newData = extraData.emplace_back();
    newData.resize(wavData.size() - currOffset, 0);
    newData.shrink_to_fit();

    std::memcpy(newData.data(), wavData.data() + currOffset, wavData.size() - currOffset);
    recordMap.try_emplace(currOffset, Hitman4WAVRecord{newData, currOffset});
  }

  header = reinterpret_cast<Hitman4WAVHeader *>(recordMap.at(0).data.data());

  path = loadPath;

  return true;
}

bool Hitman4WAVFile::Save(const StringView8CI savePathView)
{
  const auto savePath = savePathView.path();
  create_directories(savePath.parent_path());

  const auto oldSync = std::ios_base::sync_with_stdio(false);

  std::ofstream wavData(savePath, std::ios::binary | std::ios::trunc);

  uint32_t offset = 0;
  for (auto &record : recordMap | std::views::values)
  {
    record.newOffset = offset;
    offset += static_cast<uint32_t>(record.data.size());
  }

  for (auto &record : recordMap | std::views::values)
    wavData.write(record.data.data(), record.data.size());

  std::ios_base::sync_with_stdio(oldSync);

  path = savePath;

  return true;
}

bool Hitman4WHDFile::Clear(const bool retVal)
{
  header = nullptr;
  recordMap.clear();
  data.clear();
  path.clear();

  return retVal;
}

bool Hitman4WHDFile::Load(Hitman4Dialog& archiveDialog, StringView8CI loadPathView)
{
  if (data.empty())
    data = ReadWholeBinaryFile(loadPathView);

  if (data.empty())
    return Clear(false);

  recordMap.clear();

  auto *whdPtr = data.data();
  header = reinterpret_cast<Hitman4WHDHeader *>(whdPtr);
  whdPtr += sizeof(Hitman4WHDHeader);

  while (*whdPtr)
  {
    whdPtr += std::strlen(whdPtr) + 1; // + 0-15 bytes, so it is aligned on 16 bytes...
    if (reinterpret_cast<uintptr_t>(whdPtr) % 16 != 0)
      whdPtr += 16 - (reinterpret_cast<uintptr_t>(whdPtr) % 16);
    while (memcmp(whdPtr, terminateBytes.data(), terminateBytes.size()) == 0)
      whdPtr += 16;

    uint32_t iterCount = 0;
    do {
      auto *whdRecord = reinterpret_cast<Hitman4WHDRecord *>(whdPtr);
      whdPtr += sizeof(Hitman4WHDRecord);

      bool nullBytesCheckPassed = true;
      for (const auto nullByte : whdRecord->mission.nullBytes)
        nullBytesCheckPassed &= nullByte == 0;
      if (!nullBytesCheckPassed && whdRecord->streams.id != 0x004F4850)
      {
        if (whdRecord->concatenated.nullByte != 0)
        {
          whdPtr = reinterpret_cast<char *>(whdRecord);
          break;
        }
        whdPtr -= 3 * sizeof(uint32_t);
      }

      String8CI filePath(std::string_view(data.data() + whdRecord->mission.filePathOffset));
      auto filePathNative = filePath.path();

      if (filePathNative.extension() != StringViewWCI(L".wav"))
        return Clear(false);

      if (whdRecord->streams.dataInStreams == 0)
        filePath = ChangeExtension(String8CI(relative(loadPathView.path(), archiveDialog.basePath.path())), filePath);
      else if (!filePathNative.has_parent_path())
        filePath = L"Streams" / filePathNative;

      auto& file = archiveDialog.GetFile(filePath);

      if (!recordMap.try_emplace(file.path, whdRecord).second)
        return Clear(false);

      archiveDialog.whdRecordsMap[file.path].emplace_back(whdRecord);
      archiveDialog.fileMap[file.path].archiveRecord = whdRecord->ToHitmanSoundRecord();

      ++iterCount;
    } while (!reinterpret_cast<uint32_t *>(whdPtr)[0] && reinterpret_cast<uint32_t *>(whdPtr)[1] && std::distance(whdPtr, &data.back() + 1) != 16);

    if (iterCount > 1)
      whdPtr += 2 * sizeof(uint32_t);
  }

  path = loadPathView;

  return true;
}

bool Hitman4WHDFile::Save(const Hitman4WAVFile &streamsWAV, const Hitman4WAVFile &missionWAV, const StringView8CI savePathView)
{
  for (auto *whdRecord : recordMap | std::views::values)
  {
    const auto &wavRecordMap = whdRecord->streams.dataInStreams == 0 ? missionWAV.recordMap : streamsWAV.recordMap;
    const auto wavRecordIt = wavRecordMap.find(whdRecord->streams.dataOffset);
    assert(wavRecordIt != wavRecordMap.end());
    if (wavRecordIt != wavRecordMap.end())
      whdRecord->streams.dataOffset = wavRecordIt->second.newOffset;
  }

  const auto oldSync = std::ios_base::sync_with_stdio(false);

  std::ofstream whdData(savePathView.path(), std::ios::binary | std::ios::trunc);
  whdData.write(data.data(), data.size());
  whdData.close();

  std::ios_base::sync_with_stdio(oldSync);

  path = savePathView;

  return true;
}

bool Hitman4Dialog::Clear(const bool retVal)
{
  whdFiles.clear();
  wavFiles.clear();
  streamsWAV.Clear();
  basePath.clear();
  fileMap.clear();
  whdRecordsMap.clear();

  return HitmanDialog::Clear(retVal);
}

bool Hitman4Dialog::ImportSingle(const StringView8CI importFolderPath, const StringView8CI importFilePath)
{
  String8CI filePath = relative(importFilePath.path(), importFolderPath.path());
  auto fileIt = fileMap.find(filePath);
  auto whdRecordsIt = whdRecordsMap.find(filePath);
  if (fileIt == fileMap.end() || whdRecordsIt == whdRecordsMap.end())
  {
    const StringView8CI nextExtension = filePath.path().extension() == StringViewWCI(L".wav") ? ".ogg" : ".wav";
    filePath = ChangeExtension(filePath, nextExtension);
    fileIt = fileMap.find(filePath);
    whdRecordsIt = whdRecordsMap.find(filePath);
    if (fileIt == fileMap.end() || whdRecordsIt == whdRecordsMap.end())
    {
      DisplayWarning(LocalizationManager::Get().LocalizeFormat("HITMAN_DIALOG_WARNING_MISSING_FILE", importFilePath));
      return false;
    }
  }

  if (!ImportSingleHitmanFile(fileIt->second, filePath, importFilePath))
    return false;

  for (auto* whdRecord : whdRecordsIt->second)
    whdRecord->FromHitmanSoundRecord(fileIt->second.archiveRecord);

  return true;
}

bool Hitman4Dialog::LoadImpl(const StringView8CI loadPath)
{
  Clear();

  const auto rootPath = loadPath.path().parent_path();
  const auto scenesPath = rootPath / L"Scenes";
  if (!exists(scenesPath))
  {
    DisplayError(LocalizationManager::Get().Localize("HITMAN_23_DIALOG_ERROR_MISSING_SCENES"));
    return false;
  }

  const auto allWHDFiles = GetAllFilesInDirectory(String8CI(scenesPath), ".whd", true);

  whdFiles.reserve(allWHDFiles.size());
  wavFiles.reserve(allWHDFiles.size());

  basePath = rootPath;

  OrderedMap<StringView8CI, Hitman4WHDRecord *> allWHDRecords;
  for (const auto &whdPath : allWHDFiles)
  {
    auto &whdFile = whdFiles.emplace_back();
    if (!whdFile.Load(*this, whdPath))
      return Clear(false);

    for (const auto& whdRecordMapKV : whdFile.recordMap)
      allWHDRecords.insert(whdRecordMapKV);

    if (!wavFiles.emplace_back().Load(ChangeExtension(whdFile.path, ".wav"), whdFile.recordMap, fileMap, true))
      return Clear(false);
  }

  if (!streamsWAV.Load(loadPath, allWHDRecords, fileMap, false))
    return Clear(false);

  if (!options.common.checkOriginality)
    return true;

  auto dataPath = GetProgramPath().path();
  if (dataPath.empty())
    return Clear(false);

  dataPath /= L"data";
  dataPath /= L"records";
  dataPath /= L"h4";

  originalDataPath = dataPath;

  if (!LoadOriginalData())
    return Clear(false);

  return true;
}

bool Hitman4Dialog::SaveImpl(const StringView8CI savePathView)
{
  const auto newBasePath = savePathView.path().parent_path();

  streamsWAV.Save(savePathView);

  for (size_t i = 0; i < whdFiles.size(); ++i)
  {
    String8CI savePath = newBasePath / relative(wavFiles[i].path.path(), basePath.path());
    wavFiles[i].Save(savePath);
    whdFiles[i].Save(streamsWAV, wavFiles[i], savePath);
  }

  basePath = newBasePath;

  CleanDirty();

  return true;
}

void Hitman4Dialog::DrawDialog()
{
  DrawHitmanDialog("Blood Money", "Hitman 4 Streams (pc_eng.str)\0pc_eng.str\0", "pc_eng.str");
}
