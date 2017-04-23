namespace NFieldClient {


class TMoveTransaction {
    void Move(TGrainId, const TVector& to);
    void Apply();
};


class IFieldActor {
public:
    const TGrain& See(const TPoint& pt, const TVector& to) const;

    TMoveTransaction CreateMoveTransaction();
};


}  // NFieldClient
