import { CanFrame } from "../interfaces/can";

type Props = {
  data: CanFrame[];
};

function Table({ data }: Props) {
  return (
    <table className="w-4/6">
      <thead>
        <tr>
          <th className="text-left">Identifier</th>
          <th className="text-left">Length</th>
          <th className="text-left">RTR</th>
          <th className="text-left">Extended</th>
          <th className="text-left">Frequency</th>
        </tr>
      </thead>
      <tbody>
        {data.map((item) => (
          <tr key={item.id} className="border-t border-t-slate-600 hover:bg-slate-600">
            <td className="text-left">{item.id}</td>
            <td className="text-left">{item.length}</td>
            <td className="text-left">{item.isRTR ? "Yes" : "No"}</td>
            <td className="text-left">{item.isExtended ? "Yes" : "No"}</td>
            <td className="text-left">{item.frequency}</td>
          </tr>
        ))}
      </tbody>
    </table>
  );
}

export default Table;
