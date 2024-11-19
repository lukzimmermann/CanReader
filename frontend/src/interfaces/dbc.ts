export interface DbcCanFrame {
  id: number;
  name: string;
  length: number;
  signals: DbcCanMessage[];
}

export interface DbcCanMessage {
  name: string;
  startBit: number;
  length: number;
  isLittleEndian: boolean;
  isUnsigned: boolean;
  scale: number;
  offset: number;
  unit: string;
}
