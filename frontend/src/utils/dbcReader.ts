import { DbcCanFrame, DbcCanMessage } from "../interfaces/dbc";

export function parseDbcFile(stringData: string): DbcCanFrame[] {
  const lines = stringData.replace(/�/g, "°").split("\n");
  const dbcMessages: DbcCanFrame[] = [];

  for (const line of lines) {
    if (line.trimStart().startsWith("BO_ ")) {
      const segments = line.split(" ");
      const dbcMessage = {
        id: Number(segments[1]),
        name: segments[2].replace(/:/g, ""),
        length: Number(segments[3]),
        signals: [],
      } as DbcCanFrame;
      dbcMessages.push(dbcMessage);
    }

    if (line.trimStart().startsWith("SG_ ")) {
      const segments = line.split(" ");
      const { startBit, length, isLittleEndian, isUnsigned } = parseBitInformation(segments[4]);
      const { scale, offset } = parseScaleInformation(segments[5]);
      const dbcSignal = {
        name: segments[2],
        startBit: startBit,
        length: length,
        isUnsigned: isUnsigned,
        isLittleEndian: isLittleEndian,
        scale: scale,
        offset: offset,
        unit: segments[7].replace(/"/g, ""),
      } as DbcCanMessage;
      dbcMessages[-1]?.signals.push(dbcSignal);
    }
  }
  console.log(JSON.stringify(dbcMessages, null, 2));
  return dbcMessages;
}

function parseBitInformation(stringElement: string): {
  startBit: number;
  length: number;
  isLittleEndian: boolean;
  isUnsigned: boolean;
} {
  const elements = stringElement.split(/[\|@]/);
  return {
    startBit: Number(elements[0]),
    length: Number(elements[1]),
    isLittleEndian: elements[2][0] == "1" ? true : false,
    isUnsigned: elements[2][1] == "+" ? true : false,
  };
}

function parseScaleInformation(stringElement: string): {
  scale: number;
  offset: number;
} {
  const elements = stringElement.replace(/[()']/g, "").split(",");
  return { scale: Number(elements[0]), offset: Number(elements[1]) };
}
