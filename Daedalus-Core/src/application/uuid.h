#pragma once

namespace daedalus {

	// This could be exapanded to two uint64_t (total of 128 bit value) if collisions become an issue
	// however the chance of a collision is small

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

}

namespace std {

	template <typename T> struct hash;

	template<>
	struct hash<daedalus::UUID>
	{
		std::size_t operator()(const daedalus::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}