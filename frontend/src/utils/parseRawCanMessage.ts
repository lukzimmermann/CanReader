import { CanFrame, CanMessage, CanMessageSignal } from "../interfaces/can";
import { DbcCanFrame, DbcCanMessage } from "../interfaces/dbc";

export function parseRawCanMessages(data: CanFrame[], dbc: DbcCanFrame[]) {
  const canMessages: CanMessage[] = [];

  for (const canFrame of data) {
    const dbcItem = getDbcItem(canFrame.id, dbc);
    if (dbcItem) {
      const canMessage: CanMessage = {
        id: canFrame.id,
        name: dbcItem.name,
        length: canFrame.length,
        signals: [],
        data: canFrame.data,
        isRTR: canFrame.isRTR,
        isExtended: canFrame.isExtended,
        frequency: canFrame.frequency,
      };
      for (const dbcSignal of dbcItem.signals) {
        const value = calculateValue(canFrame.data, dbcSignal);
        const signal = {
          name: dbcSignal.name,
          value: value,
          unit: dbcSignal.unit,
        } as CanMessageSignal;
        canMessage.signals.push(signal);
      }
      canMessages.push(canMessage);
    }
  }

  return canMessages;
}

function getDbcItem(id: number, dbc: DbcCanFrame[]) {
  for (const dbcItem of dbc) {
    if (dbcItem.id === id) return dbcItem;
  }
  //console.error(`${id} not found`);
}

function calculateValue(data: number[], dbcSignal: DbcCanMessage) {
  const signalData = getSignalData(data, dbcSignal);
  const rawValue = signalData.reduce((combinedValue, byte) => (combinedValue << 8) | byte, 0);
  return rawValue * dbcSignal.scale + dbcSignal.offset;
}

function getSignalData(data: number[], dbcSignal: DbcCanMessage) {
  const startByte = Math.floor(dbcSignal.startBit / 8);
  const endByte = startByte + dbcSignal.length / 8;
  const signalData = data.slice(startByte, endByte);

  if (dbcSignal.isLittleEndian) {
    return signalData.reverse();
  } else {
    return signalData;
  }
}
